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

SH_DECL_HOOK3_void(ISource2Server, GameFrame, SH_NOATTRIB, false, bool, bool, bool);
SH_DECL_HOOK1_void(ISource2GameClients, ClientFullyConnect, SH_NOATTRIB, false, CPlayerSlot);
SH_DECL_HOOK5(ISource2GameClients, ProcessUsercmds, SH_NOATTRIB, false, float, CPlayerSlot, bf_read *, int, bool, bool);
SH_DECL_HOOK2_void(ISource2GameClients, ClientCommand, SH_NOATTRIB, false, CPlayerSlot, const CCommand &);
StubPlugin g_StubPlugin;
ISource2GameClients *gameclients = NULL;
ISource2Server *gamedll = NULL;
ISource2ServerConfig *serverconfig = NULL;
IVEngineServer2 *engine = NULL;
CGlobalVars* gpGlobals = NULL;
#include "kztimer.h"
PLUGIN_EXPOSE(StubPlugin, g_StubPlugin);

bool gBEnableJumpDebug = false;
f32 gF_Tickrate = 64.0f;

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
	SH_ADD_HOOK(ISource2GameClients, ClientCommand, gameclients, SH_STATIC(Hook_ClientCommand), false);


	// windows
	char *serverbin = "../../csgo/bin/win64/server.dll";
	// PlayerSlotToPlayerController
	{
		char *sig = "\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\x27\x27\x27\x27\x48\x85\xC0\x74\x3D";
		char *mask = "xxxxxxxxx....xxxxx";
		if (!(PlayerSlotToPlayerController = (PlayerSlotToPlayerController_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
	}
	
	// CEntityInstance_entindex
	{
		char *sig = "\x40\x53\x48\x83\xEC\x20\x4C\x8B\x41\x10\x48\x8B\xDA";
		char *mask = "xxxxxxxxxxxxx";
		if (!(CEntityInstance_entindex = (CEntityInstance_entindex_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
	}
	
	{
		char *sig = "\x48\x8B\xC4\x48\x89\x48\x08\x55\x53\x56\x57\x41\x54\x41\x56\x41";
		char *mask = "xxxxxxxxxxxxxxxx";
		if (!(CCSPP_PostThink = (CCSPP_PostThink_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSPP_PostThink_hook = subhook_new((void *)CCSPP_PostThink, Hook_CCSPP_PostThink, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSPP_PostThink_hook);
	}
	
	{
		char *sig = "\x48\x89\x5C\x24\x18\x56\x48\x83\xEC\x40\x48\x8B\xF2\x48";
		char *mask = "xxxxxxxxxxxxxx";
		if (!(CCSP_MS__CheckJumpButton = (CCSP_MS__CheckJumpButton_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__CheckJumpButton_hook = subhook_new((void *)CCSP_MS__CheckJumpButton, Hook_CCSP_MS__CheckJumpButton, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__CheckJumpButton_hook);
	}
	
	{
		char *sig = "\x40\x53\x57\x48\x81\xEC\xA8\x00\x00\x00\x48\x8B\xD9";
		char *mask = "xxxxxxxxxxxxx";
		if (!(CCSP_MS__OnJump = (CCSP_MS__OnJump_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__OnJump_hook = subhook_new((void *)CCSP_MS__OnJump, Hook_CCSP_MS__OnJump, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__OnJump_hook);
	}

	{
		char* sig = "\xF3\x0F\x10\x05\x58\xA0\x7F\x00";
		char* mask = "xxxxxxxx";
		if (!(CCSGC__GetTickInterval = (CCSGC__GetTickInterval_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSGC__GetTickInterval_Hook = subhook_new((void*)CCSGC__GetTickInterval, Hook_CCSGC__GetTickInterval, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSGC__GetTickInterval_Hook);
	}

	{
		char* sig = "\x48\x83\xEC\x38\x33\xC0\x48\x8B\xD1\x48\x89\x44\x24\x28\x45\x33\xC9\x45\x33\xC0\x48\x89\x44\x24\x20\x8D\x48\x04";
		char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		if (!(PrintCenter_Server = (ScriptPrintCenter_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		PrintCenter_Server_Hook = subhook_new((void*)PrintCenter_Server, Hook_PrintCenter_Server, SUBHOOK_64BIT_OFFSET);
		subhook_install(PrintCenter_Server_Hook);
	}
	{
		char* sig = "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x18\x48\x89\x7C\x24\x20\x41\x56\x48\x81\xEC\x90\x00\x00\x00";
		char* mask = "xxxxxxxxxxxxxxxxxxxxxxxx";
		if (!(PrintCenter = (PrintCenter_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		PrintCenter_Hook = subhook_new((void*)PrintCenter, Hook_PrintCenter, SUBHOOK_64BIT_OFFSET);
		subhook_install(PrintCenter_Hook);
	}
	{
		char* sig = "\x40\x56\x57\x48\x81\xEC\xA8\x00\x00\x00\x48\x8B\xF9";
		char* mask = "xxxxxxxxxxxxx";
		if (!(CCSP_MS__ProcessMovement = (CCSP_MS__ProcessMovement_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__ProcessMovement_hook = subhook_new((void*)CCSP_MS__ProcessMovement, Hook_CCSP_MS__ProcessMovement, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__ProcessMovement_hook);
	}
	{
		char* sig = "\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x30\x80\xB9\xC2\x02\x00\x00\x00";
		char* mask = "xxxxxxxxxxxxxxxxx";
		if (!(CCSPP_GetMaxSpeed = (CCSPP_GetMaxSpeed_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSPP_GetMaxSpeed_hook = subhook_new((void*)CCSPP_GetMaxSpeed, Hook_CCSPP_GetMaxSpeed, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSPP_GetMaxSpeed_hook);
	}
	
	{
		char *sig = "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x56\x57\x41\x56\x48\x83\xEC\x40\x4D";
		char *mask = "xxxxxxxxxxxxxxxxxxx";
		if (!(CreateEntity = (CreateEntity_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CreateEntity_hook = subhook_new((void *)CreateEntity, Hook_CreateEntity, SUBHOOK_64BIT_OFFSET);
		subhook_install(CreateEntity_hook);
	}
	
	return true;
}


bool StubPlugin::Unload(char *error, size_t maxlen)
{
	SH_REMOVE_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_REMOVE_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientCommand, gameclients, SH_STATIC(Hook_ClientCommand), false);
	
	subhook_remove(CCSPP_PostThink_hook);
	subhook_free(CCSPP_PostThink_hook);
	
	subhook_remove(CCSP_MS__CheckJumpButton_hook);
	subhook_free(CCSP_MS__CheckJumpButton_hook);
	
	subhook_remove(CCSP_MS__WalkMove_hook);
	subhook_free(CCSP_MS__WalkMove_hook);
	
	subhook_remove(CreateEntity_hook);
	subhook_free(CreateEntity_hook);
	
	return true;
}

void DoPrintCenter(CCSPlayerPawn* pawn, const char* fmt, ...)
{
	va_list ap;
	char buffer[2048];

	va_start(ap, fmt);
	int len = vsnprintf(buffer, sizeof(buffer), fmt, ap);
	if (len >= 2048)
	{
		len = 2047;
		buffer[len] = '\0';
	}
	va_end(ap);
	PrintCenter_Server(buffer);
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
	if (gBEnableJumpDebug && gpGlobals->tickcount % 64 == 0) META_CONPRINTF("%i PostThink  %x\n", gpGlobals->tickcount, this_);
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
	//if (gBEnableJumpDebug)	META_CONPRINTF("(PRE) [%i (%.6f)] m_flJumpUntil %f | m_flJumpPressedTime %f, | delta %f\n", gpGlobals->tickcount, gpGlobals->curtime, this_->m_flJumpUntil, this_->m_flJumpPressedTime, gpGlobals->curtime - this_->m_flJumpPressedTime);
	CCSP_MS__CheckJumpButton(this_, mv);
	//if (gBEnableJumpDebug)	META_CONPRINTF("(POST) [%i (%.6f)] m_flJumpUntil %f | m_flJumpPressedTime %f, | delta %f\n", gpGlobals->tickcount, gpGlobals->curtime, this_->m_flJumpUntil, this_->m_flJumpPressedTime, gpGlobals->curtime - this_->m_flJumpPressedTime);

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
	float lastJumpUntil = this_->m_flJumpUntil;
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
		gBEnableJumpDebug = !gBEnableJumpDebug;
		DoPrintCenter(NULL, "gBEnableJumpDebug %s\n", gBEnableJumpDebug ? "on" : "off");
		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "gettickrate") == 0)
	{
		META_CONPRINTF("Current tickrate: %f", gF_Tickrate);
		RETURN_META(MRES_SUPERCEDE);
	}
	else if (strcmp(args[0], "settickrate") == 0 && args.ArgC() == 2)
	{
		gF_Tickrate = clamp(atof(args[1]), 10, 1000);
		META_CONPRINTF("New tickrate: %f. Please change the map to apply the new tickrate.\n", gF_Tickrate);
		RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_IGNORED);
}

internal CCSGC__GETTICKINTERVAL(Hook_CCSGC__GetTickInterval)
{
	return clamp(1 / gF_Tickrate, 0.001, 0.1);
}

internal PRINTCENTER(Hook_PrintCenter)
{
	subhook_remove(PrintCenter_Hook);
	//const char* newmsg = "Hello!";
	PrintCenter(pawn, msg_dest, msg_name, param1, param2, param3, param4);
	//if (gBEnableJumpDebug && msg_dest == 4) META_CONPRINTF("PrintCenter %x | Dest %i | Name %s %i %i %i %i\n", pawn, msg_dest, msg_name, param1, param2, param3, param4);
	subhook_install(PrintCenter_Hook);
}

internal SCRIPTPRINTCENTER(Hook_PrintCenter_Server)
{
	subhook_remove(PrintCenter_Server_Hook);
	//const char* newmsg = "Hello!";
	PrintCenter_Server(unk);
	//META_CONPRINTF("PrintCenter_Server %s", unk);
	subhook_install(PrintCenter_Server_Hook);
}


internal CCSP_MS__PROCESSMOVEMENT(Hook_CCSP_MS__ProcessMovement)
{
	subhook_remove(CCSP_MS__ProcessMovement_hook);
	CCSP_MS__ProcessMovement(this_, mv);
	//if (gBEnableJumpDebug) META_CONPRINTF("%i %f\n", gpGlobals->tickcount, mv->m_vecVelocity.Length2D());
	if (gpPawn != NULL && this_->pawn == gpPawn)
	{
		char buffer[1024] = "";
		g_RealVelPreMod = CalcPrestrafeVelMod(this_, mv);
		strcat(buffer, GetSpeedText(this_, mv));
		strcat(buffer, "\n");
		strcat(buffer, GetKeyText(this_, mv));
		DoPrintCenter(NULL, buffer);
		oldEyeAngles = mv->m_vecViewAngles;
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

