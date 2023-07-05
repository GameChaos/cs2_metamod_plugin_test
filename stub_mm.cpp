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

#include "gc_common.h"

#ifdef _WIN32
#include <windows.h>
#include <psapi.h>
#else
#error Only windows supported.
#endif

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

typedef void *PlayerSlotToPlayerController_t(CPlayerSlot slot);
PlayerSlotToPlayerController_t *PlayerSlotToPlayerController = NULL;

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
	char *signature = "\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\x27\x27\x27\x27\x48\x85\xC0\x74\x3D";
	char *mask = "xxxxxxxxx....xxxxx";
	
#ifdef _WIN32
	char *binPath = "../../csgo/bin/win64/server.dll";
	HMODULE bin = LoadLibrary(binPath);
	
	MODULEINFO info = {0};
	GetModuleInformation(GetCurrentProcess(), bin, &info, sizeof(info));
	
	s64 binBytes = info.SizeOfImage;
#elif __linux__
	// untested
	char *binPath = "server.so";
	auto *bin = dlopen(binPath, RTLD_NOW);
	
	s64 binBytes = MEGABYTES(50);
#endif
	
	if (!bin)
	{
		snprintf(error, maxlen, "Could not open %s", binPath);
	}
	
	PlayerSlotToPlayerController = (PlayerSlotToPlayerController_t *)SigScan(bin, binBytes, signature, mask);
	
	return true;
}

bool StubPlugin::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_REMOVE_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	
	return true;
}

void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick)
{
	gpGlobals = engine->GetServerGlobals();
}

void Hook_ClientFullyConnect(CPlayerSlot slot)
{
	void *test = PlayerSlotToPlayerController(slot);
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

uintptr_t SigScan(void *start, size_t maxScanBytes, char *pattern, char *ignorePattern)
{
	uintptr_t patternLen = strlen(ignorePattern);
	
	char *memory = (char *)start;
	for (uintptr_t i = 0; i < maxScanBytes; i++)
	{
		uintptr_t matches = 0;
		while (memory[i + matches] == pattern[matches] || ignorePattern[matches] != 'x')
		{
			matches++;
			if (matches == 14)
			{
				matches = matches;
			}
			if (matches == patternLen)
			{
				return (uintptr_t)(memory + i);
			}
		}
	}
	return NULL;
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
