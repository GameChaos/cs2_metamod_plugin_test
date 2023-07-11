/**
 * vim: set ts=4 sw=4 tw=99 noet :
 * ======================================================
 * Metamod:Source Stub Plugin
 * Written by AlliedModders LLC.
 * ======================================================
 *
 * This software is provided 'as-is', without any express or implied warranty.
 * In no event will the authors be held liable for any damages arising from 
 * the use of this software.
 *
 * This stub plugin is public domain.
 */

#define META_IS_SOURCE2 1
#define SOURCE_ENGINE 28

#include <stdio.h>

#define SIGSCAN_IMPLEMENTATION
#include "sigscan.h"

#include "stub_mm.h"
#include "gc_common.h"
#include "cs2_datatypes.h"
#include "cs2_gamemovement.h"

#include "subhook/subhook.h"

StubPlugin g_StubPlugin;
ISource2GameClients *gameclients = NULL;
ISource2Server *gamedll = NULL;
ISource2ServerConfig *serverconfig = NULL;
IVEngineServer2 *engine = NULL;
CGlobalVars *gpGlobals = NULL;

#include "hooks.cpp"

#define MAXPLAYERS 64

#define PRE_VELMOD_MAX 1.104 // Calculated 276/250

struct PlayerData
{
	b32 turning;
	
	// CCSPlayerPawnBase *pawn;
	QAngle oldAngles;
	
	f32 realPreVelMod;
	f32 preVelMod;
	f32 preVelModLastChange;
	s32 preTickCounter;
};

PlayerData g_playerData[MAXPLAYERS + 1];

PLUGIN_EXPOSE(StubPlugin, g_StubPlugin);
bool StubPlugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetServerFactory, gamedll, ISource2Server, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_CURRENT(GetServerFactory, serverconfig, ISource2ServerConfig, INTERFACEVERSION_SERVERCONFIG);
	GET_V_IFACE_CURRENT(GetServerFactory, gameclients, ISource2GameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	
	return Hooks_HookFunctions(error, maxlen);
}

bool StubPlugin::Unload(char *error, size_t maxlen)
{
	Hooks_UnhookFunctions();
	
	return true;
}

internal float VectorNormalize(Vector &vec)
{
	float result = sqrtf(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
	if (result != 0)
	{
		vec *= 1.0f / result;
	}
	return result;
}

internal b32 IsButtonDown(CInButtonState buttons, InputBitMask_t button)
{
	b32 result = buttons.m_pButtonStates[0] & button;
	return result;
}

float GetCurtime()
{
	gpGlobals = engine->GetServerGlobals();
	float result = gpGlobals->curtime;
	return result;
}

float GetClientMovingDirection(CCSPlayer_MovementServices *moveServices, CMoveData *mv, bool ladder)
{
	QAngle angles = mv->m_vecViewAngles;
	angles[0] = CLAMP(-70.0f, angles[0], 70.0f);
	
	Vector viewNormal;
	if (ladder)
	{
		viewNormal = moveServices->m_vecLadderNormal;
	}
	else
	{
		AngleVectors_(&mv->m_vecViewAngles, &viewNormal, NULL, NULL);
	}
	
	Vector velocity = mv->m_vecVelocity.Normalized();
	
	f32 direction = velocity.Dot(viewNormal);
	if (ladder)
	{
		direction *= -1;
	}
	return direction;
}

float CalcPrestrafeVelMod(PlayerData *pd, CCSPlayer_MovementServices *moveServices, CMoveData *mv)
{
	if (!CBaseEntity_GetGroundEntity(moveServices->pawn))
	{
		return pd->preVelMod;
	}
	float speed = mv->m_vecVelocity.Length2D();
	
	u64 buttons = moveServices->m_nButtons.m_pButtonStates[0];
	b32 turning = mv->m_vecViewAngles[1] != pd->oldAngles[1];
	if (!turning)
	{
		if (GetCurtime() - pd->preVelModLastChange > 0.2f)
		{
			pd->preVelMod = 1.0f;
			pd->preVelModLastChange = GetCurtime();
		}
		else if (pd->preVelMod > PRE_VELMOD_MAX + 0.007f)
		{
			return PRE_VELMOD_MAX - 0.001f; // Returning without setting the variable is intentional
		}
	}
	else if ((buttons & IN_MOVELEFT || buttons & IN_MOVERIGHT) && speed > 248.9)
	{
		float increment = 0.0009f;
		if (pd->preVelMod > 1.04f)
		{
			increment = 0.001f;
		}
		
		bool forwards = GetClientMovingDirection(moveServices, mv, false) > 0.0f;
		
		if ((buttons & IN_MOVERIGHT && buttons || buttons && !forwards)
			 || (buttons & IN_MOVELEFT && buttons || buttons && !forwards))
		{
			pd->preTickCounter++;
			
			if (pd->preTickCounter < 75)
			{
				pd->preVelMod += increment;
				if (pd->preVelMod > PRE_VELMOD_MAX)
				{
					if (pd->preVelMod > PRE_VELMOD_MAX + 0.007f)
					{
						pd->preVelMod = PRE_VELMOD_MAX - 0.001f;
					}
					else
					{
						pd->preVelMod -= 0.007f;
					}
				}
				pd->preVelMod += increment;
			}
			else
			{
				pd->preVelMod -= 0.0045;
				pd->preTickCounter -= 2;
				
				if (pd->preVelMod < 1.0)
				{
					pd->preVelMod = 1.0;
					pd->preTickCounter = 0;
				}
			}
		}
		else
		{
			pd->preVelMod -= 0.04;
			
			if (pd->preVelMod < 1.0)
			{
				pd->preVelMod = 1.0;
			}
		}
		
		pd->preVelModLastChange = GetCurtime();
	}
	else
	{
		pd->preTickCounter = 0;
		return 1.0; // Returning without setting the variable is intentional
	}
	
	return pd->preVelMod;
}

internal b32 IsValidPlayerSlot(CPlayerSlot slot)
{
	b32 result = slot.Get() >= 0 && slot.Get() <= MAXPLAYERS;
	return result;
}

internal CEntityIndex GetPawnControllerEntIndex(CBasePlayerPawn *pawn)
{
	CEntityIndex result = -1;
	if (pawn->m_hController.m_Index != 0xffffffff)
	{
		result = pawn->m_hController.m_Index & 0x3fff;
	}
	return result;
}

internal CPlayerSlot GetPawnPlayerSlot(CBasePlayerPawn *pawn)
{
	CEntityIndex entindex = GetPawnControllerEntIndex(pawn);
	CPlayerSlot result = -1;
	if (entindex.Get() > 0 && entindex.Get() <= MAXPLAYERS + 1)
	{
		result = entindex.Get() - 1;
	}
	return result;
}

internal CBasePlayerController *GetPawnController(CBasePlayerPawn *pawn)
{
	CBasePlayerController *result = NULL;
	CEntityIndex index = GetPawnControllerEntIndex(pawn);
	// TODO: what is maxplayers?
	if (index.Get() > 0)
	{
		result = PlayerSlotToPlayerController(CPlayerSlot(index.Get() - 1));
	}
	return result;
}

// TODO: is this CBasePlayerPawn or CCSPlayerPawnBase?!
internal CCSPLAYERPAWN_POSTTHINK(Hook_CCSPP_PostThink)
{
	subhook_remove(CCSPP_PostThink_hook);
	CCSPP_PostThink(this_);
	
	CPlayerSlot slot = GetPawnPlayerSlot(this_);
	if (IsValidPlayerSlot(slot))
	{
		PlayerData *pd = &g_playerData[slot.Get()];
		
		pd->oldAngles = this_->m_angEyeAngles;
	}
	
	subhook_install(CCSPP_PostThink_hook);
}

internal CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton)
{
	subhook_remove(CCSP_MS__CheckJumpButton_hook);
	CCSP_MS__CheckJumpButton(this_, mv);
	gpGlobals = engine->GetServerGlobals();
	
	// META_CONPRINTF("[%i (%.6f)]m_flJumpUntil %f\n", gpGlobals->tickcount, gpGlobals->curtime, this_->m_flJumpUntil);
	subhook_install(CCSP_MS__CheckJumpButton_hook);
}

internal CCSP_MS__WALKMOVE(Hook_CCSP_MS__WalkMove)
{
	subhook_remove(CCSP_MS__WalkMove_hook);
	
	CCSP_MS__WalkMove(this_, mv);
	
	CPlayerSlot slot = GetPawnPlayerSlot(this_->pawn);
	if (IsValidPlayerSlot(slot))
	{
		PlayerData *pd = &g_playerData[slot.Get()];
		pd->realPreVelMod = CalcPrestrafeVelMod(pd, this_, mv);
	}
	
	subhook_install(CCSP_MS__WalkMove_hook);
}

internal CCSPP_GETMAXSPEED(Hook_CCSPP_GetMaxSpeed)
{
	CPlayerSlot slot = GetPawnPlayerSlot(this_);
	if (IsValidPlayerSlot(slot))
	{
		PlayerData *pd = &g_playerData[slot.Get()];
		return 250.0f * pd->realPreVelMod;
	}
	return 250.0f;
}

internal CCSP_MS__FRICTION(Hook_CCSP_MS__Friction)
{
	subhook_remove(CCSP_MS__Friction_hook);
	
	CCSP_MS__Friction(this_, mv);
	
	subhook_install(CCSP_MS__Friction_hook);
}

internal CREATEENTITY(Hook_CreateEntity)
{
	subhook_remove(CreateEntity_hook);
	CBaseEntity *result = CreateEntity(this_, a2, class_, memory, zero, a6, a7, a8);
	
	if (result && strcmp(result->m_pEntity->m_designerName.String(), "func_button") == 0)
	{
		CBaseButton *button = (CBaseButton *)result;
		button = button;
	}
	
	subhook_install(CreateEntity_hook);
	return result;
}

internal void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	gpGlobals = engine->GetServerGlobals();
}

internal void ResetPlayerData(PlayerData *pd)
{
	*pd = PlayerData{};
}

internal void Hook_ClientFullyConnect(CPlayerSlot slot)
{
	ResetPlayerData(&g_playerData[slot.Get()]);
}

internal float Hook_ProcessUsercmds(CPlayerSlot slot, bf_read *buf, int numcmds, bool ignore, bool paused)
{
	
	RETURN_META_VALUE(MRES_IGNORED, 0.0f);
}

void StubPlugin::AllPluginsLoaded()
{
	/* This is where we'd do stuff that relies on the mod or other plugins 
	 * being initialized (for example, cvars added and events registered).
	 */
}

bool StubPlugin::Pause(char *error, size_t maxlen)
{
	return true;
}

bool StubPlugin::Unpause(char *error, size_t maxlen)
{
	return true;
}

const char *StubPlugin::GetLicense()
{
	return "Public Domain";
}

const char *StubPlugin::GetVersion()
{
	return "0.0.0";
}

const char *StubPlugin::GetDate()
{
	return __DATE__;
}

const char *StubPlugin::GetLogTag()
{
	return "GC";
}

const char *StubPlugin::GetAuthor()
{
	return "GameChaos";
}

const char *StubPlugin::GetDescription()
{
	return "Sample empty plugin";
}

const char *StubPlugin::GetName()
{
	return "Stub Plugin";
}

const char *StubPlugin::GetURL()
{
	return "http://www.sourcemm.net/";
}
