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

StubPlugin g_StubPlugin;
ISource2GameClients *gameclients = NULL;
ISource2Server *gamedll = NULL;
ISource2ServerConfig *serverconfig = NULL;
IVEngineServer2 *engine = NULL;
CGlobalVars *gpGlobals = NULL;

#include "hooks.cpp"
bool enableDebug = false;
f32 tickrate = 64.0f;
#include "util.cpp"
#include "kztimer.cpp"

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

internal CEntityIndex GetPawnControllerEntIndex(CBasePlayerPawn *pawn)
{
	CEntityIndex result = -1;
	if (pawn->m_hController.m_Index != 0xffffffff)
	{
		CEntityIndex result = pawn->m_hController.m_Index & 0x3fff;
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

internal CBASEPLAYERPAWN_POSTTHINK(Hook_CCSPP_PostThink)
{
	subhook_remove(CCSPP_PostThink_hook);
	CCSPP_PostThink(this_);
	gpGlobals = engine->GetServerGlobals();
	if (enableDebug && gpGlobals->tickcount % 64 == 0) META_CONPRINTF("%i PostThink  %x\n", gpGlobals->tickcount, this_);
	gpPawn = this_;
	
	CEntityIndex entindex = -1;
	CEntityInstance_entindex(this_, &entindex);
	
	CEntityIndex index = GetPawnControllerEntIndex(this_);
	CBasePlayerController *controller = GetPawnController(this_);
	
	subhook_install(CCSPP_PostThink_hook);
}

internal CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton)
{
	subhook_remove(CCSP_MS__CheckJumpButton_hook);

	gpGlobals = engine->GetServerGlobals();
	//if (enableDebug)	META_CONPRINTF("(PRE) [%i (%.6f)] m_flJumpUntil %f | m_flJumpPressedTime %f, | delta %f\n", gpGlobals->tickcount, gpGlobals->curtime, this_->m_flJumpUntil, this_->m_flJumpPressedTime, gpGlobals->curtime - this_->m_flJumpPressedTime);
	CCSP_MS__CheckJumpButton(this_, mv);
	//if (enableDebug)	META_CONPRINTF("(POST) [%i (%.6f)] m_flJumpUntil %f | m_flJumpPressedTime %f, | delta %f\n", gpGlobals->tickcount, gpGlobals->curtime, this_->m_flJumpUntil, this_->m_flJumpPressedTime, gpGlobals->curtime - this_->m_flJumpPressedTime);

	subhook_install(CCSP_MS__CheckJumpButton_hook);
}

internal CCSP_MS__WALKMOVE(Hook_CCSP_MS__WalkMove)
{
	subhook_remove(CCSP_MS__WalkMove_hook);
	f32 oldMaxspeed = mv->m_flMaxSpeed;
	mv->m_flMaxSpeed = 250.0f * CalcPrestrafeVelMod(this_, mv);
	CCSP_MS__WalkMove(this_, mv);
	mv->m_flMaxSpeed = oldMaxspeed;
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
		gfPrespeed = mv->m_vecVelocity.Length2D();
	}

	
	subhook_install(CCSP_MS__OnJump_hook);
}

internal void Hook_ClientCommand(CPlayerSlot slot, const CCommand& args)
{
	META_CONPRINTF("ClientCommand: %s\n", args.GetCommandString());
	if (strcmp(args[0], "toggledebug") == 0)
	{
		enableDebug = !enableDebug;
		DoPrintCenter("enableDebug %s\n", enableDebug ? "on" : "off");
		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "gettickrate") == 0)
	{
		META_CONPRINTF("Current tickrate: %f", tickrate);
		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "settickrate") == 0 && args.ArgC() == 2)
	{
		tickrate = clamp(atof(args[1]), 10, 1000);
		META_CONPRINTF("New tickrate: %f. Please change the map to apply the new tickrate.\n", tickrate);
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
	CCSP_MS__ProcessMovement(this_, mv);
	//if (enableDebug) META_CONPRINTF("%i %f\n", gpGlobals->tickcount, mv->m_vecVelocity.Length2D());
	if (gpPawn != NULL && this_->pawn == gpPawn)
	{
		char buffer[1024] = "";
		g_RealVelPreMod = CalcPrestrafeVelMod(this_, mv);
		strcat(buffer, GetSpeedText(this_, mv));
		strcat(buffer, "\n");
		strcat(buffer, GetKeyText(this_, mv));
		DoPrintCenter(buffer);
		UpdateOldEyeAngles(mv);
	}
	subhook_install(CCSP_MS__ProcessMovement_hook);
}

internal CCSPP_GETMAXSPEED(Hook_CCSPP_GetMaxSpeed)
{
	return 250.0f * g_RealVelPreMod;
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

internal void Hook_ClientFullyConnect(CPlayerSlot slot)
{
	CBasePlayerController* test = PlayerSlotToPlayerController(slot);
	META_CONPRINTF("Controller for %i: %x\n", slot, test);
	gpGlobals = engine->GetServerGlobals();
	META_CONPRINTF("player slot: %i\n", slot.Get());
	META_CONPRINTF("network version: %i\n", serverconfig->GetNetworkVersion());
	META_CONPRINTF("realtime: %f\n", gpGlobals->realtime);
	META_CONPRINTF("framecount: %i\n", gpGlobals->framecount);
	META_CONPRINTF("absoluteframetime: %f\n", gpGlobals->absoluteframetime);
	META_CONPRINTF("absoluteframestarttimestddev: %f\n", gpGlobals->absoluteframestarttimestddev);
	META_CONPRINTF("maxClients: %i\n", gpGlobals->maxClients);
	META_CONPRINTF("interpolation_amount: %f\n", gpGlobals->interpolation_amount);
	META_CONPRINTF("simTicksThisFrame: %i\n", gpGlobals->simTicksThisFrame);
	META_CONPRINTF("network_protocol: %i\n", gpGlobals->network_protocol);
	META_CONPRINTF("MaybeCheckCurTime: %p\n", gpGlobals->MaybeCheckCurtime);
	META_CONPRINTF("frametime: %f\n", gpGlobals->frametime);
	META_CONPRINTF("curtime: %f\n", gpGlobals->curtime);
	META_CONPRINTF("m_bClient: %i\n", gpGlobals->m_bClient);
	META_CONPRINTF("nTimestampNetworkingBase: %i\n", gpGlobals->nTimestampNetworkingBase);
	META_CONPRINTF("nTimestampNetworkingWindow: %i\n", gpGlobals->nTimestampRandomizeWindow);
	META_CONPRINTF("tickcount: %i\n", gpGlobals->tickcount);
	META_CONPRINTF("interval_per_tick: %f\n", gpGlobals->interval_per_tick);
	META_CONPRINTF("mapname: %s\n", gpGlobals->mapname);
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

