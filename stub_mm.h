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

#ifndef _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
#define _INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_

#include <ISmmPlugin.h>
#define SIGSCAN_IMPLEMENTATION
#include "sigscan.h"
#include "gc_common.h"
#include "cs2_datatypes.h"
#include "cs2_gamemovement.h"

#include "subhook/subhook.h"


#if defined WIN32 && !defined snprintf
#define snprintf _snprintf
#endif

class StubPlugin : public ISmmPlugin
{
public:
	bool Load(PluginId id, ISmmAPI *ismm, char *error, size_t maxlen, bool late);
	bool Unload(char *error, size_t maxlen);
	bool Pause(char *error, size_t maxlen);
	bool Unpause(char *error, size_t maxlen);
	void AllPluginsLoaded();
public:
	const char *GetAuthor();
	const char *GetName();
	const char *GetDescription();
	const char *GetURL();
	const char *GetLicense();
	const char *GetVersion();
	const char *GetDate();
	const char *GetLogTag();
};

void Hook_ServerActivate(edict_t *pEdictList, int edictCount, int clientMax);
void Hook_GameFrame(bool simulating, bool bFirstTick, bool bLastTick);
void Hook_ClientFullyConnect(CPlayerSlot slot);
float Hook_ProcessUsercmds(CPlayerSlot slot, bf_read* buf, int numcmds, bool ignore, bool paused);
void Hook_ClientCommand(CPlayerSlot slot, const CCommand& args);

uintptr_t FindPattern(void* start, size_t maxScanBytes, char* pattern, char* ignorePattern);

typedef CBasePlayerController* PlayerSlotToPlayerController_t(CPlayerSlot slot);
PlayerSlotToPlayerController_t* PlayerSlotToPlayerController = NULL;


#define CBASEPLAYERPAWN_POSTTHINK(name) void name(CCSPlayerPawn *this_)
typedef CBASEPLAYERPAWN_POSTTHINK(CCSPP_PostThink_t);
CCSPP_PostThink_t* CCSPP_PostThink = NULL;
subhook_t CCSPP_PostThink_hook;
internal CBASEPLAYERPAWN_POSTTHINK(Hook_CCSPP_PostThink);

#define CCSP_MS__CHECKJUMPBUTTON(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__CHECKJUMPBUTTON(CCSP_MS__CheckJumpButton_t);
CCSP_MS__CheckJumpButton_t* CCSP_MS__CheckJumpButton = NULL;
subhook_t CCSP_MS__CheckJumpButton_hook;
internal CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton);

#define CCSP_MS__WALKMOVE(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__WALKMOVE(CCSP_MS__WalkMove_t);
CCSP_MS__WalkMove_t* CCSP_MS__WalkMove = NULL;
subhook_t CCSP_MS__WalkMove_hook;
internal CCSP_MS__WALKMOVE(Hook_CCSP_MS__WalkMove);

#define CCSP_MS__ONJUMP(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__ONJUMP(CCSP_MS__OnJump_t);
CCSP_MS__OnJump_t* CCSP_MS__OnJump = NULL;
subhook_t CCSP_MS__OnJump_hook;
internal CCSP_MS__ONJUMP(Hook_CCSP_MS__OnJump);

#define CCSGC__GETTICKINTERVAL(name) float name(void *this_)
typedef CCSGC__GETTICKINTERVAL(CCSGC__GetTickInterval_t);
CCSGC__GetTickInterval_t* CCSGC__GetTickInterval = NULL;
subhook_t CCSGC__GetTickInterval_Hook;
internal CCSGC__GETTICKINTERVAL(Hook_CCSGC__GetTickInterval);

#define PRINTCENTER(name) void name(void *pawn, int msg_dest, const char* msg_name, const char* param1, const char* param2, const char* param3, const char* param4)
typedef PRINTCENTER(PrintCenter_t);
PrintCenter_t* PrintCenter = NULL;
subhook_t PrintCenter_Hook;
internal PRINTCENTER(Hook_PrintCenter);

#define SCRIPTPRINTCENTER(name) void name(void* unk)
typedef SCRIPTPRINTCENTER(ScriptPrintCenter_t);
ScriptPrintCenter_t* PrintCenter_Server = NULL;
subhook_t PrintCenter_Server_Hook;
internal SCRIPTPRINTCENTER(Hook_PrintCenter_Server);


#define CCSP_MS__PROCESSMOVEMENT(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__PROCESSMOVEMENT(CCSP_MS__ProcessMovement_t);
CCSP_MS__ProcessMovement_t* CCSP_MS__ProcessMovement = NULL;
subhook_t CCSP_MS__ProcessMovement_hook;
internal CCSP_MS__PROCESSMOVEMENT(Hook_CCSP_MS__ProcessMovement);

#define CCSPP_GETMAXSPEED(name) float name(CCSPlayerPawn* this_)
typedef CCSPP_GETMAXSPEED(CCSPP_GetMaxSpeed_t);
CCSPP_GetMaxSpeed_t* CCSPP_GetMaxSpeed = NULL;
subhook_t CCSPP_GetMaxSpeed_hook;
internal CCSPP_GETMAXSPEED(Hook_CCSPP_GetMaxSpeed);

CCSPlayerPawn* gpPawn = NULL;
extern StubPlugin g_StubPlugin;
void DoPrintCenter(CCSPlayerPawn *pawn, const char* fmt, ...);
PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
