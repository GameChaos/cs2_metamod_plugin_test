
SH_DECL_HOOK3_void(ISource2Server, GameFrame, SH_NOATTRIB, false, bool, bool, bool);
SH_DECL_HOOK1_void(ISource2GameClients, ClientFullyConnect, SH_NOATTRIB, false, CPlayerSlot);
// SH_DECL_HOOK5_void(ISource2GameClients, ClientDisconnect, SH_NOATTRIB, false, int, const char *, uint64, const char *);
SH_DECL_HOOK5(ISource2GameClients, ProcessUsercmds, SH_NOATTRIB, false, float, CPlayerSlot, bf_read *, int, bool, bool);
SH_DECL_HOOK2_void(ISource2GameClients, ClientCommand, SH_NOATTRIB, false, CPlayerSlot, const CCommand&);

typedef CBasePlayerController *PlayerSlotToPlayerController_t(CPlayerSlot slot);
PlayerSlotToPlayerController_t *PlayerSlotToPlayerController = NULL;

typedef void CEntityInstance_entindex_t(CEntityInstance *this_, CEntityIndex *out);
CEntityInstance_entindex_t *CEntityInstance_entindex = NULL;

typedef void PrintCenterTextToAll_t(char* buffer);
PrintCenterTextToAll_t *PrintCenterTextToAll = NULL;

typedef CBaseEntity *CGameEntitySystem__EntityByIndex_t(CGameEntitySystem* entSystem, int index);
CGameEntitySystem__EntityByIndex_t *CGameEntitySystem__EntityByIndex = NULL;

#define CCSPLAYERPAWN_POSTTHINK(name) void name(CCSPlayerPawnBase *this_)
typedef CCSPLAYERPAWN_POSTTHINK(CCSPP_PostThink_t);
CCSPP_PostThink_t *CCSPP_PostThink = NULL;
subhook_t CCSPP_PostThink_hook;
internal CCSPLAYERPAWN_POSTTHINK(Hook_CCSPP_PostThink);

#define CCSP_MS__CHECKJUMPBUTTON(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__CHECKJUMPBUTTON(CCSP_MS__CheckJumpButton_t);
CCSP_MS__CheckJumpButton_t *CCSP_MS__CheckJumpButton = NULL;
subhook_t CCSP_MS__CheckJumpButton_hook;
internal CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton);

#define CCSP_MS__ONJUMP(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__ONJUMP(CCSP_MS__OnJump_t);
CCSP_MS__OnJump_t *CCSP_MS__OnJump = NULL;
subhook_t CCSP_MS__OnJump_hook;
internal CCSP_MS__ONJUMP(Hook_CCSP_MS__OnJump);

#define CCSP_MS__PROCESSMOVEMENT(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__PROCESSMOVEMENT(CCSP_MS__ProcessMovement_t);
CCSP_MS__ProcessMovement_t *CCSP_MS__ProcessMovement = NULL;
subhook_t CCSP_MS__ProcessMovement_hook;
internal CCSP_MS__PROCESSMOVEMENT(Hook_CCSP_MS__ProcessMovement);

#define CCSPP_GETMAXSPEED(name) f32 name(CCSPlayerPawn* this_)
typedef CCSPP_GETMAXSPEED(CCSPP_GetMaxSpeed_t);
CCSPP_GetMaxSpeed_t *CCSPP_GetMaxSpeed = NULL;
subhook_t CCSPP_GetMaxSpeed_hook;
internal CCSPP_GETMAXSPEED(Hook_CCSPP_GetMaxSpeed);

#define CCSP_MS__WALKMOVE(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__WALKMOVE(CCSP_MS__WalkMove_t);
CCSP_MS__WalkMove_t *CCSP_MS__WalkMove = NULL;
subhook_t CCSP_MS__WalkMove_hook;
internal CCSP_MS__WALKMOVE(Hook_CCSP_MS__WalkMove);

#define CCSP_MS__AIRACCELERATE(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv, Vector& wishdir, float wishspeed, float accel)
typedef CCSP_MS__AIRACCELERATE(CCSP_MS__AirAccelerate_t);
CCSP_MS__AirAccelerate_t* CCSP_MS__AirAccelerate = NULL;
subhook_t CCSP_MS__AirAccelerate_hook;
internal CCSP_MS__AIRACCELERATE(Hook_CCSP_MS__AirAccelerate);

#define CCSGC__GETTICKINTERVAL(name) f32 name(void *this_)
typedef CCSGC__GETTICKINTERVAL(CCSGC__GetTickInterval_t);
CCSGC__GetTickInterval_t *CCSGC__GetTickInterval = NULL;
subhook_t CCSGC__GetTickInterval_hook;
internal CCSGC__GETTICKINTERVAL(Hook_CCSGC__GetTickInterval);

#define CCSP_MS__FRICTION(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__FRICTION(CCSP_MS__Friction_t);
CCSP_MS__Friction_t *CCSP_MS__Friction = NULL;
subhook_t CCSP_MS__Friction_hook;
internal CCSP_MS__FRICTION(Hook_CCSP_MS__Friction);

#define CREATEENTITY(name) CBaseEntity *name(void *this_, u32 a2, void *class_, void *memory, s32 zero, u32 a6, u32 a7, bool a8)
typedef CREATEENTITY(CreateEntity_t);
CreateEntity_t *CreateEntity = NULL;
subhook_t CreateEntity_hook;
internal CREATEENTITY(Hook_CreateEntity);

#define FINDUSEENTITY(name) CBaseEntity *name(CBasePlayerPawn* pawn)
typedef FINDUSEENTITY(FindUseEntity_t);
FindUseEntity_t *FindUseEntity = NULL;
subhook_t FindUseEntity_hook;
internal FINDUSEENTITY(Hook_FindUseEntity);

#define INITIALISEGAMEENTITYSYSTEM(name) CGameEntitySystem *name(void *memory)
typedef INITIALISEGAMEENTITYSYSTEM(InitialiseGameEntitySystem_t);
InitialiseGameEntitySystem_t *InitialiseGameEntitySystem = NULL;
subhook_t InitialiseGameEntitySystem_hook;
internal INITIALISEGAMEENTITYSYSTEM(Hook_InitialiseGameEntitySystem);

internal bool Hooks_HookFunctions(char *error, size_t maxlen)
{
	SH_ADD_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
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

	// PrintCenterTextToAll
	{
		char* sig = "\x48\x83\xEC\x38\x33\xC0\x48\x8B\xD1\x48\x89\x44\x24\x28\x45\x33\xC9\x45\x33\xC0\x48\x89\x44\x24\x20\x8D\x48\x04";
		char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		if (!(PrintCenterTextToAll = (PrintCenterTextToAll_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		};
	}
	
	// CGameEntitySystem__EntityByIndex
	{
		char *sig = "\x81\xFA\xFE\x7F\x00\x00\x77\x36";
		char *mask = "xxxxxxxx";
		if (!(CGameEntitySystem__EntityByIndex = (CGameEntitySystem__EntityByIndex_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
	}
	
	// Detours:
	
	// CCSPP_PostThink
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
	
	// CCSP_MS__CheckJumpButton
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
	
	// CCSP_MS__OnJump
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
	
	// CCSP_MS__ProcessMovement
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
	
	// CCSPP_GetMaxSpeed
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
		
	// CCSP_MS__WalkMove
	{
		char *sig = "\x48\x8B\xC4\x48\x89\x58\x18\x48\x89\x70\x20\x55\x57\x41\x54\x48\x8D";
		char *mask = "xxxxxxxxxxxxxxxxx";
		if (!(CCSP_MS__WalkMove = (CCSP_MS__WalkMove_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__WalkMove_hook = subhook_new((void *)CCSP_MS__WalkMove, Hook_CCSP_MS__WalkMove, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__WalkMove_hook);
	}

	// CCSP_MS__AirAccelerate
	{
		char* sig = "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x55\x48\x8D\x6C\x24\xB1";
		char* mask = "xxxxxxxxxxxxxxxxxxxxx";
		if (!(CCSP_MS__AirAccelerate = (CCSP_MS__AirAccelerate_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__AirAccelerate_hook = subhook_new((void*)CCSP_MS__AirAccelerate, Hook_CCSP_MS__AirAccelerate, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__AirAccelerate_hook);
	}
	
	// CCSGameConfiguration::GetTickInterval
	{
		char* sig = "\xF3\x0F\x10\x05\x58\xA0\x7F\x00";
		char* mask = "xxxxxxxx";
		if (!(CCSGC__GetTickInterval = (CCSGC__GetTickInterval_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSGC__GetTickInterval_hook = subhook_new((void*)CCSGC__GetTickInterval, Hook_CCSGC__GetTickInterval, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSGC__GetTickInterval_hook);
	}

	// CCSP_MS__Friction
	{
		char *sig = "\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x60\x48\x8B\x41\x30";
		char *mask = "xxxxxxxxxxxxxx";
		if (!(CCSP_MS__Friction = (CCSP_MS__Friction_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__Friction_hook = subhook_new((void *)CCSP_MS__Friction, Hook_CCSP_MS__Friction, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__Friction_hook);
	}
	
	// CCSP_MS__AirAccelerate
	{
		char *sig = "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x48\x89\x7C\x24\x18\x55\x48\x8D\x6C\x24\xB1";
		char *mask = "xxxxxxxxxxxxxxxxxxxxx";
		if (!(CCSP_MS__AirAccelerate = (CCSP_MS__AirAccelerate_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__AirAccelerate_hook = subhook_new((void *)CCSP_MS__AirAccelerate, Hook_CCSP_MS__AirAccelerate, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__AirAccelerate_hook);
	}
	
	// CreateEntity
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
	
	
	// FindUseEntity
	{
		char* sig = "\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x55\x57\x41\x54\x41\x55\x41\x57\x48\x8D\xAC\x24\x50\xEB\xFF\xFF";
		char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxx";
		if (!(FindUseEntity = (FindUseEntity_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		};
		FindUseEntity_hook = subhook_new((void*)FindUseEntity, Hook_FindUseEntity, SUBHOOK_64BIT_OFFSET);
		subhook_install(FindUseEntity_hook);
	}
	
	// CCSPP_GetMaxSpeed
	{
		char *sig = "\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x30\x80\xB9\xC2\x02\x00\x00\x00";
		char *mask = "xxxxxxxxxxxxxxxxx";
		if (!(CCSPP_GetMaxSpeed = (CCSPP_GetMaxSpeed_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSPP_GetMaxSpeed_hook = subhook_new((void*)CCSPP_GetMaxSpeed, Hook_CCSPP_GetMaxSpeed, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSPP_GetMaxSpeed_hook);
	}
	
	// InitialiseGameEntitySystem
	{
		char *sig = "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x33\xFF\xC7\x83\x10";
		char *mask = "xxxxxxxxxxxxxxxxxxx????xxxxx";
		if (!(InitialiseGameEntitySystem = (InitialiseGameEntitySystem_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		InitialiseGameEntitySystem_hook = subhook_new((void *)InitialiseGameEntitySystem, Hook_InitialiseGameEntitySystem, SUBHOOK_64BIT_OFFSET);
		subhook_install(InitialiseGameEntitySystem_hook);
	}

	// InitialiseGameEntitySystem
	{
		char* sig = "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\xCC\xCC\xCC\xCC\x33\xFF\xC7\x83\x10";
		char* mask = "xxxxxxxxxxxxxxxxxxx????xxxxx";
		if (!(InitialiseGameEntitySystem = (InitialiseGameEntitySystem_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		};
		InitialiseGameEntitySystem_hook = subhook_new((void*)InitialiseGameEntitySystem, Hook_InitialiseGameEntitySystem, SUBHOOK_64BIT_OFFSET);
		subhook_install(InitialiseGameEntitySystem_hook);
	}
	return true;
}

internal void Hooks_UnhookFunctions()
{
	SH_REMOVE_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_REMOVE_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientCommand, gameclients, SH_STATIC(Hook_ClientCommand), false);
	
	subhook_remove(CCSPP_PostThink_hook);
	subhook_free(CCSPP_PostThink_hook);
	
	subhook_remove(CCSP_MS__CheckJumpButton_hook);
	subhook_free(CCSP_MS__CheckJumpButton_hook);
	
	subhook_remove(CCSP_MS__OnJump_hook);
	subhook_free(CCSP_MS__OnJump_hook);
	
	subhook_remove(CCSP_MS__ProcessMovement_hook);
	subhook_free(CCSP_MS__ProcessMovement_hook);
	
	subhook_remove(CCSPP_GetMaxSpeed_hook);
	subhook_free(CCSPP_GetMaxSpeed_hook);
	
	subhook_remove(CCSP_MS__WalkMove_hook);
	subhook_free(CCSP_MS__WalkMove_hook);

	subhook_remove(CCSP_MS__Friction_hook);
	subhook_free(CCSP_MS__Friction_hook);
	
	subhook_remove(CCSP_MS__AirAccelerate_hook);
	subhook_free(CCSP_MS__AirAccelerate_hook);

	subhook_remove(CCSGC__GetTickInterval_hook);
	subhook_free(CCSGC__GetTickInterval_hook);

	subhook_remove(FindUseEntity_hook);
	subhook_free(FindUseEntity_hook);

	subhook_remove(CreateEntity_hook);
	subhook_free(CreateEntity_hook);

	subhook_remove(InitialiseGameEntitySystem_hook);
	subhook_free(InitialiseGameEntitySystem_hook);
}