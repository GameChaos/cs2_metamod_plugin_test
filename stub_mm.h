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

#include "mathlib/mathlib.h"

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

#define MAXPLAYERS 64

enum TurnState
{
	TURN_LEFT = -1,
	TURN_NONE = 0,
	TURN_RIGHT = 1
};

class Checkpoint
{
public:
	Vector origin;
	QAngle angles;
	Vector ladderNormal;
	bool onLadder;
	CHandle< CBaseEntity > groundEnt; // 0x3c4

	Checkpoint()
	{
	}
	Checkpoint(CCSPlayerPawn* pawn)
	{
		this->origin = pawn->m_pSceneNode->m_vecAbsOrigin;
		this->angles = pawn->v_angle;
		this->ladderNormal = static_cast<CCSPlayer_MovementServices*>(pawn->m_pMovementServices)->m_vecLadderNormal;
		this->onLadder = pawn->m_MoveType == MOVETYPE_LADDER;
		this->groundEnt = pawn->m_hGroundEntity;
	}
};

class PlayerData
{
public:
	// General
	b32 turning;
	f32 preSpeed;

	// CCSPlayerPawnBase *pawn;
	QAngle oldAngles;

	// KZT stuff
	f32 realPreVelMod;
	f32 preVelMod;
	f32 preVelModLastChange;
	f32 preCounter;

	// Timer stuff
	f32 timerStartTime;
	b32 timerRunning;
	
	// CP/Teleport stuff
	CUtlVector<Checkpoint> checkpoints;
	f32 teleportTime;
	Vector lastOrigin;
	QAngle lastAngle;

	void MakeCheckpoint(CCSPlayerPawn* pawn)
	{
		int i = this->checkpoints.AddToTail();
		this->checkpoints[i] = Checkpoint(pawn);
	}
};

extern StubPlugin g_StubPlugin;
PLUGIN_GLOBALVARS();

#endif //_INCLUDE_METAMOD_SOURCE_STUB_PLUGIN_H_
