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

internal b32 IsButtonDown(CInButtonState buttons, InputBitMask_t button)
{
	b32 result = buttons.m_pButtonStates[0] & button;
	return result;
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
	
	CEntityIndex entindex = -1;
	CEntityInstance_entindex(this_, &entindex);
	
	CEntityIndex index = GetPawnControllerEntIndex(this_);
	CBasePlayerController *controller = GetPawnController(this_);
	
	if (this_->m_fFlags & FL_ONGROUND)
	{
		// this_->m_vecAbsVelocity.z = 320.0f;
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
	gpGlobals = engine->GetServerGlobals();
	
	if (IsButtonDown(this_->m_nButtons, IN_FORWARD))
	{
		
	}
	
	subhook_install(CCSP_MS__WalkMove_hook);
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
	CBasePlayerController *test = PlayerSlotToPlayerController(slot);
	engine->ClientCommand(slot, "say poopee");
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
