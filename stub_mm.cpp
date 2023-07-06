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

SH_DECL_HOOK3_void(ISource2Server, GameFrame, SH_NOATTRIB, false, bool, bool, bool);
SH_DECL_HOOK1_void(ISource2GameClients, ClientFullyConnect, SH_NOATTRIB, false, CPlayerSlot);
SH_DECL_HOOK5(ISource2GameClients, ProcessUsercmds, SH_NOATTRIB, false, float, CPlayerSlot, bf_read *, int, bool, bool);

StubPlugin g_StubPlugin;
ISource2GameClients *gameclients = NULL;
ISource2Server *gamedll = NULL;
ISource2ServerConfig *serverconfig = NULL;
IVEngineServer2 *engine = NULL;
CGlobalVars* gpGlobals = NULL;

void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick);
void Hook_ClientFullyConnect(CPlayerSlot slot);
float Hook_ProcessUsercmds(CPlayerSlot slot, bf_read *buf, int numcmds, bool ignore, bool paused);

uintptr_t FindPattern(void *start, size_t maxScanBytes, char *pattern, char *ignorePattern);

typedef CBaseEntity *PlayerSlotToPlayerController_t(CPlayerSlot slot);
PlayerSlotToPlayerController_t *PlayerSlotToPlayerController = NULL;

#define CBASEPLAYERPAWN_POSTTHINK(name) void name(CBaseEntity *this_)
typedef CBASEPLAYERPAWN_POSTTHINK(CCSPlayerPawn_PostThink_t);
CCSPlayerPawn_PostThink_t *CCSPlayerPawn_PostThink = NULL;
subhook_t CCSPlayerPawn_PostThink_hook;
CBASEPLAYERPAWN_POSTTHINK(Hook_CCSPlayerPawn_PostThink);

#define CCSP_MS__CHECKJUMPBUTTON(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__CHECKJUMPBUTTON(CCSP_MS__CheckJumpButton_t);
CCSP_MS__CheckJumpButton_t *CCSP_MS__CheckJumpButton = NULL;
subhook_t CCSP_MS__CheckJumpButton_hook;
CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton);


PLUGIN_EXPOSE(StubPlugin, g_StubPlugin);
bool StubPlugin::Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	PLUGIN_SAVEVARS();
	
	GET_V_IFACE_CURRENT(GetEngineFactory, engine, IVEngineServer2, INTERFACEVERSION_VENGINESERVER);
	GET_V_IFACE_CURRENT(GetServerFactory, gamedll, ISource2Server, INTERFACEVERSION_SERVERGAMEDLL);
	GET_V_IFACE_CURRENT(GetServerFactory, serverconfig, ISource2ServerConfig, INTERFACEVERSION_SERVERCONFIG);
	GET_V_IFACE_CURRENT(GetServerFactory, gameclients, ISource2GameClients, INTERFACEVERSION_SERVERGAMECLIENTS);
	
	SH_ADD_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_ADD_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_ADD_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	
	// windows
	char *serverbin = "../../csgo/bin/win64/server.dll";
	
	char *signature = "\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\x27\x27\x27\x27\x48\x85\xC0\x74\x3D";
	char *mask = "xxxxxxxxx....xxxxx";
	
	PlayerSlotToPlayerController = (PlayerSlotToPlayerController_t *)SigScan(serverbin, signature, mask, error, maxlen);
	if (PlayerSlotToPlayerController == NULL)
	{
		return false;
	}
	
	char *CCSPlayerPawn_PostThink_sig = "\x48\x8B\xC4\x48\x89\x48\x08\x55\x53\x56\x57\x41\x54\x41\x56\x41";
	char *CCSPlayerPawn_PostThink_mask = "xxxxxxxxxxxxxxxx";
	CCSPlayerPawn_PostThink = (CCSPlayerPawn_PostThink_t *)SigScan(serverbin,
		CCSPlayerPawn_PostThink_sig, CCSPlayerPawn_PostThink_mask, error, maxlen);
	if (CCSPlayerPawn_PostThink == NULL)
	{
		return false;
	}
	CCSPlayerPawn_PostThink_hook = subhook_new((void *)CCSPlayerPawn_PostThink,
		Hook_CCSPlayerPawn_PostThink, SUBHOOK_64BIT_OFFSET);
	subhook_install(CCSPlayerPawn_PostThink_hook);
	
	char *CCSP_MS__CheckJumpButton_sig = "\x48\x89\x5C\x24\x18\x56\x48\x83\xEC\x40\x48\x8B\xF2\x48";
	char *CCSP_MS__CheckJumpButton_mask = "xxxxxxxxxxxxxx";
	
	CCSP_MS__CheckJumpButton = (CCSP_MS__CheckJumpButton_t *)SigScan(serverbin,
		CCSP_MS__CheckJumpButton_sig, CCSP_MS__CheckJumpButton_mask, error, maxlen);
	if (CCSP_MS__CheckJumpButton == NULL)
	{
		return false;
	}
	CCSP_MS__CheckJumpButton_hook = subhook_new((void *)CCSP_MS__CheckJumpButton,
		Hook_CCSP_MS__CheckJumpButton, SUBHOOK_64BIT_OFFSET);
	subhook_install(CCSP_MS__CheckJumpButton_hook);
	
	return true;
}

bool StubPlugin::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_REMOVE_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	
	subhook_remove(CCSPlayerPawn_PostThink_hook);
	subhook_free(CCSPlayerPawn_PostThink_hook);
	
	return true;
}

CBASEPLAYERPAWN_POSTTHINK(Hook_CCSPlayerPawn_PostThink)
{
	subhook_remove(CCSPlayerPawn_PostThink_hook);
	CCSPlayerPawn_PostThink(this_);
	if (this_->m_fFlags & FL_ONGROUND)
	{
		// this_->m_vecAbsVelocity.z = 320.0f;
	}
	
	subhook_install(CCSPlayerPawn_PostThink_hook);
}

CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton)
{
	subhook_remove(CCSP_MS__CheckJumpButton_hook);
	CCSP_MS__CheckJumpButton(this_, mv);
	
	subhook_install(CCSP_MS__CheckJumpButton_hook);
}

void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	gpGlobals = engine->GetServerGlobals();
}

void Hook_ClientFullyConnect(CPlayerSlot slot)
{
	CBaseEntity *test = PlayerSlotToPlayerController(slot);
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

float Hook_ProcessUsercmds(CPlayerSlot slot, bf_read *buf, int numcmds, bool ignore, bool paused)
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
