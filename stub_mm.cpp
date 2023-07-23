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
#include "stub_mm.h"

class CGameEntitySystem;

StubPlugin g_StubPlugin;
ISource2GameClients *gameclients = NULL;
ISource2Server *gamedll = NULL;
ISource2ServerConfig *serverconfig = NULL;
IVEngineServer2 *engine = NULL;
extern ICvar *g_pCVar;
extern IGameUIService* g_pGameUIService;
CGlobalVars *gpGlobals = NULL;
CGameEntitySystem *g_entitySystem = NULL;
PlayerData g_playerData[MAXPLAYERS + 1];

#include "hooks.cpp"
bool enableDebug = false;
f32 tickrate = 128.0f;
f32 maxspeed = 250.0f;
#include "util.cpp"
#include "kztimer.cpp"



PLUGIN_EXPOSE(StubPlugin, g_StubPlugin);

bool StubPlugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	
	MathLib_Init(2.2f, 2.2f, 0.0f, 2, false, false, false, false);
	
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetServerFactory, gamedll, ISource2Server, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_CURRENT(GetServerFactory, serverconfig, ISource2ServerConfig, INTERFACEVERSION_SERVERCONFIG);
	GET_V_IFACE_CURRENT(GetServerFactory, gameclients, ISource2GameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pCVar, ICvar, CVAR_INTERFACE_VERSION);
	GET_V_IFACE_CURRENT(GetEngineFactory, g_pGameUIService, IGameUIService, GAMEUISERVICE_INTERFACE_VERSION);

	return Hooks_HookFunctions(error, maxlen);
}


bool StubPlugin::Unload(char *error, size_t maxlen)
{
	Hooks_UnhookFunctions();
	
	return true;
}

internal CCSPLAYERPAWN_POSTTHINK(Hook_CCSPP_PostThink)
{
	subhook_remove(CCSPP_PostThink_hook);
	CCSPP_PostThink(this_);
	if (enableDebug && gpGlobals->tickcount % 64 == 0) META_CONPRINTF("%i PostThink  %x\n", gpGlobals->tickcount, this_);

	CPlayerSlot slot = GetPlayerIndex(this_);
	if (IsValidPlayerSlot(slot))
	{
		PlayerData *pd = &g_playerData[slot.Get()];
		if (pd->timerRunning && this_->m_MoveType != MOVETYPE_WALK && this_->m_MoveType != MOVETYPE_LADDER && this_->m_MoveType != MOVETYPE_OBSERVER)
		{
			pd->timerRunning = false;
			float volume = (g_pCVar->GetConVar(g_pCVar->FindConVar("volume")))->values[0].m_flValue;
			engine->ClientCommand(0, "playvol buttons/button8.wav %f", volume);
			DoPrintChat("Timer stopped.\n");
		}
		pd->oldAngles = this_->m_angEyeAngles;
	}
	
	subhook_install(CCSPP_PostThink_hook);
}

internal CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton)
{
	subhook_remove(CCSP_MS__CheckJumpButton_hook);

	CCSP_MS__CheckJumpButton(this_, mv);
	
	//if (enableDebug)	META_CONPRINTF("(POST) [%i (%.6f)] m_flJumpUntil %f | m_flJumpPressedTime %f, | delta %f\n", gpGlobals->tickcount, gpGlobals->curtime, this_->m_flJumpUntil, this_->m_flJumpPressedTime, gpGlobals->curtime - this_->m_flJumpPressedTime);

	subhook_install(CCSP_MS__CheckJumpButton_hook);
}

internal CCSP_MS__WALKMOVE(Hook_CCSP_MS__WalkMove)
{
	subhook_remove(CCSP_MS__WalkMove_hook);
	CCSP_MS__WalkMove(this_, mv);
	subhook_install(CCSP_MS__WalkMove_hook);
}

internal CCSP_MS__ONJUMP(Hook_CCSP_MS__OnJump)
{
	subhook_remove(CCSP_MS__OnJump_hook);
	f32 lastJumpUntil = this_->m_flJumpUntil;
	CCSP_MS__OnJump(this_, mv);
	f32 speed = mv->m_vecVelocity.Length2D();
	if (speed > 380.0f && this_->m_flJumpUntil > lastJumpUntil)
	{
		f32 factor = 380.0f / speed;
		mv->m_vecVelocity.x *= factor;
		mv->m_vecVelocity.y *= factor;
	}
	if (this_->m_flJumpUntil > lastJumpUntil)
	{
		CPlayerSlot slot = GetPlayerIndex(this_);
		if (IsValidPlayerSlot(slot))
		{
			PlayerData *pd = &g_playerData[slot.Get()];

			pd->preSpeed = mv->m_vecVelocity.Length2D();
		}
		if (enableDebug) META_CONPRINTF("Hook_CCSP_MS__OnJump %x | moveservice %x | pawn %x | controller %x\n", CGameEntitySystem__EntityByIndex(g_entitySystem, mv->m_nPlayerHandle.m_Index & 0x3fff), this_, this_->pawn, this_->pawn->m_hController);
	}

	subhook_install(CCSP_MS__OnJump_hook);
}

internal void Hook_ClientCommand(CPlayerSlot slot, const CCommand& args)
{
	META_CONPRINTF("ClientCommand: %s\n", args.GetCommandString());
	if (strcmp(args[0], "toggledebug") == 0)
	{
		enableDebug = !enableDebug;
		DoPrintChat("enableDebug %s\n", enableDebug ? "on" : "off");
		DoPrintCenter("enableDebug %s\n", enableDebug ? "on" : "off");
		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "gettickrate") == 0)
	{
		CBasePlayerController* controller = PlayerSlotToPlayerController(slot);
		PlayerData* pd = &g_playerData[slot.Get()];
		uint32_t handle = controller->m_hPawn.m_Index & 0x3fff;
		CCSPlayerPawn* pawn = static_cast<CCSPlayerPawn*>(CGameEntitySystem__EntityByIndex(g_entitySystem, handle));
		META_CONPRINTF("Current tickrate: %f", tickrate);
		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "settickrate") == 0 && args.ArgC() == 2)
	{
		// Engine defined min/max tickrate
		tickrate = clamp(atof(args[1]), 10, 1000);
		META_CONPRINTF("New tickrate: %f. Please change the map to apply the new tickrate.\n", tickrate);
		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "kz_checkpoint") == 0)
	{
		CBasePlayerController *controller = PlayerSlotToPlayerController(slot);
		PlayerData *pd = &g_playerData[slot.Get()];
		uint32_t handle = controller->m_hPawn.m_Index & 0x3fff;
		CCSPlayerPawn *pawn = static_cast<CCSPlayerPawn*>(CGameEntitySystem__EntityByIndex(g_entitySystem, handle));
		int i = pd->checkpoints.AddToTail();
		pd->checkpoints[i] = &Checkpoint(pawn);

		float volume = (g_pCVar->GetConVar(g_pCVar->FindConVar("volume")))->values[0].m_flValue;
		engine->ClientCommand(0, "playvol buttons/blip1.wav %f", volume);

		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "kz_gocheck") == 0)
	{
		CBasePlayerController* controller = PlayerSlotToPlayerController(slot);
		PlayerData* pd = &g_playerData[slot.Get()];
		uint32_t handle = controller->m_hPawn.m_Index & 0x3fff;
		CCSPlayerPawn* pawn = static_cast<CCSPlayerPawn*>(CGameEntitySystem__EntityByIndex(g_entitySystem, handle));
		LoadCheckpoint(pawn, pd);
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}

internal CCSGC__GETTICKINTERVAL(Hook_CCSGC__GetTickInterval)
{
	return clamp(1 / tickrate, 0.001, 0.1);
}

internal CCSP_MS__PROCESSMOVEMENT(Hook_CCSP_MS__ProcessMovement)
{
	subhook_remove(CCSP_MS__ProcessMovement_hook);
	CPlayerSlot slot = GetPlayerIndex(this_);
	if (IsValidPlayerSlot(slot))
	{
		if (mv->m_nMovementCmdsThisTick > 0 && enableDebug) META_CONPRINTF("[%f] numCmd = %i\n", gpGlobals->curtime, mv->m_nMovementCmdsThisTick);
		PlayerData *pd = &g_playerData[slot.Get()];
		pd->turning = GetTurning(pd, mv);
		pd->realPreVelMod = CalcPrestrafeVelMod(pd, this_, mv);
		char buffer[1024] = "";
		strcat(buffer, GetSpeedText(pd, this_, mv));
		strcat(buffer, "\n");
		strcat(buffer, GetKeyText(this_, mv));
		DoPrintCenter(buffer);
	}
	this_->pawn->m_bTakesDamage = false;
	CCSP_MS__ProcessMovement(this_, mv);
	subhook_install(CCSP_MS__ProcessMovement_hook);
}

internal CCSPP_GETMAXSPEED(Hook_CCSPP_GetMaxSpeed)
{
	CPlayerSlot slot = GetPlayerIndex(this_);
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

internal CCSP_MS__AIRACCELERATE(Hook_CCSP_MS__AirAccelerate)
{
	subhook_remove(CCSP_MS__AirAccelerate_hook);
	
	gpGlobals = engine->GetServerGlobals();

	f32 oldMaxSpeed = CCSPP_GetMaxSpeed((CCSPlayerPawn*)this_->pawn);
	mv->m_flMaxSpeed = MIN(250.0f, mv->m_flMaxSpeed);
	// call airaccel twice to simulate 128 tick airstrafing
	s32 slot = GetPlayerIndex(this_->pawn);
	if (gpGlobals->interval_per_tick == 1.0f / 64.0f
		&& IsValidPlayerSlot(slot))
	{
		PlayerData *pd = &g_playerData[slot];
		
		QAngle angles = mv->m_vecViewAngles;
		// normalise angles beforehand
		if (angles[0] - pd->oldAngles[0] > 180)
		{
			angles[0] -= 360.0f;
		}
		else if (angles[0] - pd->oldAngles[0] < 180)
		{
			angles[0] += 360.0f;
		}
		angles += pd->oldAngles;
		angles *= 0.5f;
		
		Vector forward, right, up;
		AngleVectors(angles, &forward, &right, &up);
		
		f32 fmove = -mv->m_flForwardMove;
		f32 smove = -mv->m_flSideMove;
		
		forward[2] = 0;
		right[2] = 0;
		VectorNormalize(forward);
		VectorNormalize(right);
		
		Vector newWishdir = Vector(0, 0, 0);
		for (s32 i = 0; i < 2; i++)
		{
			newWishdir[i] = forward[i] * fmove + right[i] * smove;
		}
		VectorNormalize(newWishdir);
		
		gpGlobals->interval_per_tick = 1.0f / 128.0f;
		CCSP_MS__AirAccelerate(this_, mv, newWishdir, maxspeed, accel);
		CCSP_MS__AirAccelerate(this_, mv, wishdir, maxspeed, accel);
		gpGlobals->interval_per_tick = 1.0f / 64.0f;
	}
	else
	{
		CCSP_MS__AirAccelerate(this_, mv, wishdir, maxspeed, accel);
	}

	mv->m_flMaxSpeed = oldMaxSpeed;
	subhook_install(CCSP_MS__AirAccelerate_hook);
}

internal CCSP_MS__TRYPLAYERMOVE(Hook_CCSP_MS__TryPlayerMove)
{
	subhook_remove(CCSP_MS__TryPlayerMove_hook);
	int result = CCSP_MS__TryPlayerMove(this_, mv, pFirstDest, pFirstTrace);
	subhook_install(CCSP_MS__TryPlayerMove_hook);
	return result;
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

internal FINDUSEENTITY(Hook_FindUseEntity)
{
	subhook_remove(FindUseEntity_hook);
	CBaseEntity *ent = FindUseEntity(us);
	s32 client = GetPlayerIndex(us->pawn);
	if (ent != NULL && ent->m_pEntity != NULL && ent->m_pEntity->m_name.String() != NULL)
	{
		PlayerData* pd = &g_playerData[client];
		if (strcmp(ent->m_pEntity->m_name.String(), "climb_startbutton") == 0)
		{
			if (gpGlobals->curtime - pd->timerStartTime > 0.5)
			{
				float volume = (g_pCVar->GetConVar(g_pCVar->FindConVar("volume")))->values[0].m_flValue;
				engine->ClientCommand(0, "playvol buttons/button9.wav %f", volume);
				pd->checkpoints.RemoveAll();
				DoPrintChat("Timer started\n");
			}
			pd->timerRunning = true;
			pd->timerStartTime = gpGlobals->curtime;
		}
		else if (strcmp(ent->m_pEntity->m_name.String(), "climb_endbutton") == 0 && pd->timerRunning)
		{
			float volume = (g_pCVar->GetConVar(g_pCVar->FindConVar("volume")))->values[0].m_flValue;
			engine->ClientCommand(0, "playvol buttons/bell1.wav %f", volume);
			char buffer[12];
			pd->timerRunning = false;
			float time = gpGlobals->curtime - pd->timerStartTime;
			int roundedTime = floor(time * 100); // Time rounded to number of centiseconds

			int centiseconds = roundedTime % 100;
			roundedTime = (roundedTime - centiseconds) / 100;
			int seconds = roundedTime % 60;
			roundedTime = (roundedTime - seconds) / 60;
			int minutes = roundedTime % 60;
			roundedTime = (roundedTime - minutes) / 60;
			int hours = roundedTime;

			if (hours == 0)
			{
				snprintf(buffer, sizeof(buffer), "%02i:%02i.%02i", minutes, seconds, centiseconds);
			}
			else
			{
				snprintf(buffer, sizeof(buffer), "%i:%02i:%02i.%02i", hours, minutes, seconds, centiseconds);
			}
			DoPrintChat("Player finished in %s\n", buffer);
		}
	}
	subhook_install(FindUseEntity_hook);
	return ent;
}

internal INITIALISEGAMEENTITYSYSTEM(Hook_InitialiseGameEntitySystem)
{
	subhook_remove(InitialiseGameEntitySystem_hook);
	
	g_entitySystem = InitialiseGameEntitySystem(memory);
	
	subhook_install(InitialiseGameEntitySystem_hook);
	return g_entitySystem;
}

internal void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	gpGlobals = engine->GetServerGlobals();
	PlayerData* pd = &g_playerData[1]; // Hardcoding
	con_nprint_t info;
	info.index = 0;
	info.time_to_live = gpGlobals->interval_per_tick;
	info.color[0] = 1.0;
	info.color[1] = 1.0;
	info.color[2] = 1.0;
	info.fixed_width_font = true;
	char buffer[128] = "";
	strcat(buffer, GetTimerText(pd));
	g_pGameUIService->Con_NXPrintf(&info, "%s\n", buffer);
}

internal void ResetPlayerData(PlayerData *pd)
{
	*pd = PlayerData{};
}


internal void Hook_ClientFullyConnect(CPlayerSlot slot)
{
	UnlockCvars();
	SetupKZTimerConvars();
	engine->ServerCommand("sv_lan true");
	engine->ServerCommand("game_mode 0");
	engine->ServerCommand("mp_warmup_offline_enabled false");
	engine->ServerCommand("mp_ignore_round_win_conditions true");
	engine->ServerCommand("mp_freezetime 0");
	engine->ServerCommand("mp_round_restart_delay 0");
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

