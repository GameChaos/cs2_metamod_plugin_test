
SH_DECL_HOOK3_void(ISource2Server, GameFrame, SH_NOATTRIB, false, bool, bool, bool);
SH_DECL_HOOK1_void(ISource2GameClients, ClientFullyConnect, SH_NOATTRIB, false, CPlayerSlot);
// SH_DECL_HOOK5_void(ISource2GameClients, ClientDisconnect, SH_NOATTRIB, false, int, const char *, uint64, const char *);
SH_DECL_HOOK5(ISource2GameClients, ProcessUsercmds, SH_NOATTRIB, false, float, CPlayerSlot, bf_read *, int, bool, bool);
SH_DECL_HOOK2_void(ISource2GameClients, ClientCommand, SH_NOATTRIB, false, CPlayerSlot, const CCommand&);

typedef CBasePlayerController *PlayerSlotToPlayerController_t(CPlayerSlot slot);
PlayerSlotToPlayerController_t *PlayerSlotToPlayerController = NULL;

typedef void CEntityInstance_entindex_t(CEntityInstance *this_, CEntityIndex *out);
CEntityInstance_entindex_t *CEntityInstance_entindex = NULL;

typedef void PrintTextToAll_t(char* buffer);
PrintTextToAll_t *PrintCenterTextToAll = NULL;
PrintTextToAll_t *PrintChatToAll = NULL;

typedef void CBaseAnimGraph__Teleport_t(CBaseAnimGraph const *this_, const Vector *newPosition, const QAngle *newAngles, const Vector *newVelocity);
CBaseAnimGraph__Teleport_t *CBaseAnimGraph__Teleport = NULL;

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
CCSP_MS__AirAccelerate_t *CCSP_MS__AirAccelerate = NULL;
subhook_t CCSP_MS__AirAccelerate_hook;
internal CCSP_MS__AIRACCELERATE(Hook_CCSP_MS__AirAccelerate);

#define CCSP_MS__TRYPLAYERMOVE(name) int name(CCSPlayer_MovementServices *this_, CMoveData *mv, Vector *pFirstDest, trace_t *pFirstTrace)
typedef CCSP_MS__TRYPLAYERMOVE(CCSP_MS__TryPlayerMove_t);
CCSP_MS__TryPlayerMove_t *CCSP_MS__TryPlayerMove = NULL;
subhook_t CCSP_MS__TryPlayerMove_hook;
internal CCSP_MS__TRYPLAYERMOVE(Hook_CCSP_MS__TryPlayerMove);

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

#define FINDUSEENTITY(name) CBaseEntity *name(CPlayer_UseServices *us)
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
	SH_ADD_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	// SH_ADD_HOOK(ISource2GameClients, ClientDisconnect, gameclients, SH_STATIC(Hook_ClientDisconnect), false);
	SH_ADD_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	SH_ADD_HOOK(ISource2GameClients, ClientCommand, gameclients, SH_STATIC(Hook_ClientCommand), false);
	
	// windows
	char *serverbin = "../../csgo/bin/win64/server.dll";
	// (*(*qword_7FFE486A0328 + 344i64))(qword_7FFE486A0328, v9, "cl_soundscape_flush\n");
	// for ( result = gpGlobals; v2 < gpGlobals->maxClients; result = gpGlobals )
	// {
	//   if ( ++v2 >= *(result + 16) )
	//     break;
	//   if ( sub_7FFE479DB650(v2) )
	//   {
	//     v7 = sub_7FFE479DB650(v2);
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
	//     v23 = EntIndex(v12, &v70);
    // Warning(
    //   "Nav link (%d/%s) width (%0.2f) less than minimum (%0.2f) at ((%0.2f,%0.2f,%0.2f)->(%0.2f,%0.2f,%0.2f))\n",
	{
		char *sig = "\x40\x53\x48\x83\xEC\x20\x4C\x8B\x41\x10\x48\x8B\xDA";
		char *mask = "xxxxxxxxxxxxx";
		if (!(CEntityInstance_entindex = (CEntityInstance_entindex_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
	}

	// PrintCenterTextToAll
	// 	  qword_7FFE484B7B48 = "ScriptPrintMessageCenterAll";
	//   v10 = 0;
	//   do
	//   {
	//     if ( *(&qword_7FFE484B7B40 + v9 + 27) != 32 )
	//       break;
	//     ++v10;
	//     --v9;
	//     BYTE1(dword_7FFE484B7B58) = v10;
	//   }
	//   while ( v9 >= 0 );
	//   qword_7FFE484B7B80 = sub_7FFE47799910; <- this function
	{
		char* sig = "\x48\x83\xEC\x38\x33\xC0\x48\x8B\xD1\x48\x89\x44\x24\x28\x45\x33\xC9\x45\x33\xC0\x48\x89\x44\x24\x20\x8D\x48\x04";
		char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		if (!(PrintCenterTextToAll = (PrintTextToAll_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		};
	}
	
	{
		char* sig = "\x48\x83\xEC\x38\x33\xC0\x48\x8B\xD1\x48\x89\x44\x24\x28\x45\x33\xC9\x45\x33\xC0\x48\x89\x44\x24\x20\x8D\x48\x03";
		char* mask = "xxxxxxxxxxxxxxxxxxxxxxxxxxxx";
		if (!(PrintChatToAll = (PrintTextToAll_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		};
	}
	// CGameEntitySystem__EntityByIndex
	// 	    || (result = sub_7FFE47F5D3F0("door_break", v4), !result) )
	//   {
	//     v13 = (*(*a2 + 56i64))(a2, "entindex", 0xFFFFFFFFi64);
	//     v6 = sub_7FFE479539C0(qword_7FFE48695008, v13); <- this function
	{
		char *sig = "\x81\xFA\xFE\x7F\x00\x00\x77\x36";
		char *mask = "xxxxxxxx";
		if (!(CGameEntitySystem__EntityByIndex = (CGameEntitySystem__EntityByIndex_t *)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
	}

	{
		char* sig = "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x27\x27\x27\x27\x48\x8B\x8B\x48\x02\x00\x00\x48\x8B\x01\xFF\x50\x40\x33\xDB";
		char* mask = "xxxxxxxxxxxxxx....xxxxxxxxxxxxxxx";
		if (!(CBaseAnimGraph__Teleport = (CBaseAnimGraph__Teleport_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
	}
	// Detours:
	
	// CCSPP_PostThink
	// [%.0f %.0f %.0f] relocated from [%.0f %.0f %.0f] in %0.1f sec (v2d = %.0f u/s)\n" "health=%d, armor=%d, helmet=%d
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
	// Look for sv_jump_spam_penalty_time
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
	// "player_jump"
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
	// "pa start %f"
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
	//*(a2 + 200) = sub_7FFE4786BD30(a1->pawn); <- this function
	//   v23 = a1->pawn;
	//   a1->m_bInStuckTest = 0;
	//   v24 = v23->m_pCameraServices;
	//   v25 = sub_7FFE47482320(v23);
	//   v26 = v25;
	//   if ( v25 )
	//   {
	//     EntIndex(v25, &v35);
	//     sub_7FFE4799A200(a1, "following entity %d", v35);
	//   }
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
	// sub_7FFE47863820(a1, a2, "FullWalkMovePreMove");
	// sub_7FFE4787B4C0(a1, a2); <- this function
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
	// Look for sv_air_max_wishspeed
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
	
	// CCSP_MS__TryPlayerMove
	// "CCSPlayer_MovementServices::TryPlayerMove() Trace ended stuck"
	{
		char* sig = "\x48\x8B\xC4\x48\x89\x58\x08\x4C\x89\x48\x20\x4C\x89\x40\x18\x48\x89\x50\x10";
		char* mask = "xxxxxxxxxxxxxxxxxxx";
		if (!(CCSP_MS__TryPlayerMove = (CCSP_MS__TryPlayerMove_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSP_MS__TryPlayerMove_hook = subhook_new((void*)CCSP_MS__TryPlayerMove, Hook_CCSP_MS__TryPlayerMove, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSP_MS__TryPlayerMove_hook);
	}
	// CCSGameConfiguration::GetTickInterval
	// CCSGameConfiguration vtable, 2 functions after one with "csgo" string
	{
		char* sig = "\xF3\x0F\x10\x05\xC8\x8A\x7F\x00";
		char* mask = "xxxxxxxx";
		if (!(CCSGC__GetTickInterval = (CCSGC__GetTickInterval_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		CCSGC__GetTickInterval_hook = subhook_new((void*)CCSGC__GetTickInterval, Hook_CCSGC__GetTickInterval, SUBHOOK_64BIT_OFFSET);
		subhook_install(CCSGC__GetTickInterval_hook);
	}

	// CCSP_MS__Friction
	// sub_7FFE47869020(a1, a2); <- this function
	// sub_7FFE47863820(a1, a2, "FullWalkMovePreMove"); 
	// sub_7FFE4787B4C0(a1, a2); <- the walkmove function
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
	
	// CreateEntity
	// "Cannot create an entity because entity class is NULL %d\n"
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
	// "Radial using: %s\n"
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
	
	// InitialiseGameEntitySystem
    //   v5 = sub_7FFE4792AE90(v4); <- this function
    // else
    //   v5 = 0;
    // sub_7FFE47D8D450(v5, "server_entities", 0, 0, 0);
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