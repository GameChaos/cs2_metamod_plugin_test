#define DECLARE_FUNCTION_HOOK(name, retType, params) \
    typedef retType name##_t params; \
    name##_t *name = NULL; \
    subhook_t name##_hook; \
    internal retType Hook_##name params;

#define DECLARE_HOOK_CCSPP(name, retType, params) DECLARE_FUNCTION_HOOK(CCSPP__##name, retType, params)
#define DECLARE_HOOK_CCSP_MS(name, retType, params) DECLARE_FUNCTION_HOOK(CCSP_MS__##name, retType, params)

#define HOOK(name, sig) {(void**)&name, sig, false, NULL, NULL}
#define DETOUR_HOOK(name, sig) {(void**)&name, sig, true, &name##_hook, (void**)&Hook_##name}
#define DETOUR_HOOK_CCSPP(name, sig) DETOUR_HOOK(CCSPP__##name, sig)
#define DETOUR_HOOK_CCSP_MS(name, sig) DETOUR_HOOK(CCSP_MS__##name, sig)


SH_DECL_HOOK3_void(ISource2Server, GameFrame, SH_NOATTRIB, false, bool, bool, bool);
SH_DECL_HOOK1_void(ISource2GameClients, ClientFullyConnect, SH_NOATTRIB, false, CPlayerSlot);
SH_DECL_HOOK5(ISource2GameClients, ProcessUsercmds, SH_NOATTRIB, false, float, CPlayerSlot, bf_read*, int, bool, bool);
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

DECLARE_FUNCTION_HOOK(CCSGC__GetTickInterval, f32, (void *this_));
DECLARE_FUNCTION_HOOK(CreateEntity, CBaseEntity*, (void *this_, u32 a2, void *class_, void *memory, s32 zero, u32 a6, u32 a7, bool a8));
DECLARE_FUNCTION_HOOK(InitialiseGameEntitySystem, CGameEntitySystem*, (void *memory));
DECLARE_FUNCTION_HOOK(FindUseEntity, CBaseEntity*, (CPlayer_UseServices* us));

DECLARE_HOOK_CCSPP(PostThink, void, (CCSPlayerPawnBase* this_));

DECLARE_HOOK_CCSP_MS(ProcessMovement, void, (CCSPlayer_MovementServices* ms, CMoveData* mv));
DECLARE_HOOK_CCSPP(GetMaxSpeed, f32, (CCSPlayerPawn* this_));
DECLARE_HOOK_CCSP_MS(PlayerMoveNew, void, (CCSPlayer_MovementServices* ms, CMoveData* mv));
DECLARE_HOOK_CCSP_MS(CheckJumpButton, void, (CCSPlayer_MovementServices *ms, CMoveData *mv));
DECLARE_HOOK_CCSP_MS(OnJump, void, (CCSPlayer_MovementServices *ms, CMoveData *mv));
DECLARE_HOOK_CCSP_MS(AirAccelerate, void, (CCSPlayer_MovementServices *ms, CMoveData *mv, Vector &wishdir, float wishspeed, float accel));
DECLARE_HOOK_CCSP_MS(WalkMove, void, (CCSPlayer_MovementServices *ms, CMoveData *mv));
DECLARE_HOOK_CCSP_MS(TryPlayerMove, int, (CCSPlayer_MovementServices* ms, CMoveData* mv, Vector *pFirstDest, trace_t *pFirstTrace));
DECLARE_HOOK_CCSP_MS(Friction, void, (CCSPlayer_MovementServices* ms, CMoveData* mv));

Hook hook[] = {
	// InitialiseGameEntitySystem
	//   v5 = sub_7FFE4792AE90(v4); <- this function
	// else
	//   v5 = 0;
	// sub_7FFE47D8D450(v5, "server_entities", 0, 0, 0);
	DETOUR_HOOK(InitialiseGameEntitySystem, "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x2A\x2A\x2A\x2A\x33\xFF\xC7\x83\x10"), 

	
	// CCSPP__GetMaxSpeed
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
	// Hook modifies engine pointer for no reason?
	DETOUR_HOOK_CCSPP(GetMaxSpeed, "\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x30\x80\xB9\xC2\x02\x00\x00\x00"),

	// PlayerSlotToPlayerController
	// (*(*qword_7FFE486A0328 + 344i64))(qword_7FFE486A0328, v9, "cl_soundscape_flush\n");
	// for ( result = gpGlobals; v2 < gpGlobals->maxClients; result = gpGlobals )
	// {
	//   if ( ++v2 >= *(result + 16) )
	//     break;
	//   if ( sub_7FFE479DB650(v2) )
	//   {
	//     v7 = sub_7FFE479DB650(v2);
	HOOK(PlayerSlotToPlayerController, "\x40\x53\x48\x83\xEC\x20\x48\x8B\x05\x2A\x2A\x2A\x2A\x48\x85\xC0\x74\x3D"),

	// CEntityInstance_entindex
	//     v23 = EntIndex(v12, &v70);
	// Warning(
	//   "Nav link (%d/%s) width (%0.2f) less than minimum (%0.2f) at ((%0.2f,%0.2f,%0.2f)->(%0.2f,%0.2f,%0.2f))\n",
	//{
	//	const char *sig = "\x40\x53\x48\x83\xEC\x20\x4C\x8B\x41\x10\x48\x8B\xDA";
	//	const char *mask = "xxxxxxxxxxxxx";
	//	if (!(CEntityInstance_entindex = (CEntityInstance_entindex_t *)SigScan(serverbin, sig, mask, error, maxlen)))
	//	{
	//		return false;
	//	}
	//}
	HOOK(CEntityInstance_entindex, "\x40\x53\x48\x83\xEC\x20\x4C\x8B\x41\x10\x48\x8B\xDA"),

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
	HOOK(PrintCenterTextToAll, "\x48\x83\xEC\x38\x33\xC0\x48\x8B\xD1\x48\x89\x44\x24\x28\x45\x33\xC9\x45\x33\xC0\x48\x89\x44\x24\x20\x8D\x48\x04"),

	// PrintChatToAll
	// qword_7FFDB0CD7340 = "ScriptPrintMessageChatAll";
	// qword_7FFDB0CD7348 = "ScriptPrintMessageChatAll";
	// v15 = 0;
	// do
	// {
	// if (*(&qword_7FFDB0CD7340 + v14 + 27) != 32)
	// 	break;
	// ++v15;
	// --v14;
	// BYTE1(dword_7FFDB0CD7358) = v15;
	// } while (v14 >= 0);
	// qword_7FFDB0CD7378 = sub_7FFDAFD237F0; <- this function
	HOOK(PrintChatToAll, "\x48\x83\xEC\x38\x33\xC0\x48\x8B\xD1\x48\x89\x44\x24\x28\x45\x33\xC9\x45\x33\xC0\x48\x89\x44\x24\x20\x8D\x48\x03"),

	// CGameEntitySystem__EntityByIndex
	// 	    || (result = sub_7FFE47F5D3F0("door_break", v4), !result) )
	//   {
	//     v13 = (*(*a2 + 56i64))(a2, "entindex", 0xFFFFFFFFi64);
	//     v6 = sub_7FFE479539C0(qword_7FFE48695008, v13); <- this function
	HOOK(CGameEntitySystem__EntityByIndex, "\x81\xFA\xFE\x7F\x00\x00\x77\x36"),

	// CBaseAnimGraph__Teleport
	// 5 functions after one with "Physics_SimulateEntity" "Server Game"
	HOOK(CBaseAnimGraph__Teleport, "\x48\x89\x5C\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x2A\x2A\x2A\x2A\x48\x8B\x8B\x48\x02\x00\x00"),

	// CCSPP_PostThink
	// [%.0f %.0f %.0f] relocated from [%.0f %.0f %.0f] in %0.1f sec (v2d = %.0f u/s)\n" "health=%d, armor=%d, helmet=%d
	DETOUR_HOOK_CCSPP(PostThink, "\x48\x8B\xC4\x48\x89\x48\x08\x55\x53\x56\x57\x41\x54\x41\x56\x41"),

	// CCSP_MS__CheckJumpButton
	// Look for sv_jump_spam_penalty_time
	DETOUR_HOOK_CCSP_MS(CheckJumpButton, "\x48\x89\x5C\x24\x18\x56\x48\x83\xEC\x40\x48\x8B\xF2\x48"),

	// CCSP_MS__OnJump
	// "player_jump"
	DETOUR_HOOK_CCSP_MS(OnJump, "\x40\x53\x57\x48\x81\xEC\x98\x00\x00\x00\x48\x8B\xD9"),

	// CCSP_MS__ProcessMovement
	// "pa start %f"
	DETOUR_HOOK_CCSP_MS(ProcessMovement, "\x40\x56\x57\x48\x81\xEC\xA8\x00\x00\x00\x4C\x8B\x49\x30"),

	// CCSP_MS__WalkMove     
	// sub_7FFE47863820(a1, a2, "FullWalkMovePreMove");
	// sub_7FFE4787B4C0(a1, a2); <- this function
	DETOUR_HOOK_CCSP_MS(WalkMove, "\x48\x8B\xC4\x48\x89\x58\x18\x48\x89\x70\x20\x55\x57\x41\x54\x48\x8D"),

	// CCSP_MS__AirAccelerate
	// Look for sv_air_max_wishspeed
	DETOUR_HOOK_CCSP_MS(AirAccelerate, "\x48\x8B\xC4\x48\x89\x58\x10\x48\x89\x70\x18\x48\x89\x78\x20\x55\x48\x8D\x68\xA9"),

	// CCSP_MS__TryPlayerMove
	// "CCSPlayer_MovementServices::TryPlayerMove() Trace ended stuck"
	DETOUR_HOOK_CCSP_MS(TryPlayerMove, "\x48\x8B\xC4\x48\x89\x58\x08\x4C\x89\x48\x20\x4C\x89\x40\x18\x48\x89\x50\x10"),

	// CCSGameConfiguration::GetTickInterval
	// CCSGameConfiguration vtable, 2 functions after one with "csgo" string
	DETOUR_HOOK(CCSGC__GetTickInterval, "\xF3\x0F\x10\x05\x18\x3C\x7E\x00"),

	// CCSP_MS__Friction
	// sub_7FFE47869020(a1, a2); <- this function
	// sub_7FFE47863820(a1, a2, "FullWalkMovePreMove"); 
	// sub_7FFE4787B4C0(a1, a2); <- the walkmove function
	DETOUR_HOOK_CCSP_MS(Friction, "\x48\x89\x74\x24\x18\x57\x48\x83\xEC\x60\x48\x8B\x41\x30"),

	// CreateEntity
	// "Cannot create an entity because entity class is NULL %d\n"
	DETOUR_HOOK(CreateEntity, "\x48\x89\x5C\x24\x08\x48\x89\x6C\x24\x10\x56\x57\x41\x56\x48\x83\xEC\x40\x4D"),

	// FindUseEntity
	// "Radial using: %s\n"
	DETOUR_HOOK(FindUseEntity, "\x48\x89\x5C\x24\x18\x48\x89\x74\x24\x20\x55\x57\x41\x54\x41\x55\x41\x56\x48\x8D\xAC\x24\x50\xEB\xFF\xFF")
};

internal bool Hooks_HookFunctions(char *error, size_t maxlen)
{
	SH_ADD_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_ADD_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_ADD_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	SH_ADD_HOOK(ISource2GameClients, ClientCommand, gameclients, SH_STATIC(Hook_ClientCommand), false);
	
	// windows
	const char *serverbin = "../../csgo/bin/win64/server.dll";

	{
		const char* sig = "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83\xEC\x20\x48\x8B\xD9\xE8\x2A\x2A\x2A\x2A\x33\xFF\xC7\x83\x10";
		const char* mask = "xxxxxxxxxxxxxxxxxxx****xxxxx";
		if (!(InitialiseGameEntitySystem = (InitialiseGameEntitySystem_t*)SigScan(serverbin, sig, mask, error, maxlen)))
		{
			return false;
		}
		InitialiseGameEntitySystem_hook = subhook_new((void*)InitialiseGameEntitySystem, Hook_InitialiseGameEntitySystem, SUBHOOK_64BIT_OFFSET);
		subhook_install(InitialiseGameEntitySystem_hook);
		subhook_remove(InitialiseGameEntitySystem_hook);
		subhook_free(InitialiseGameEntitySystem_hook);
	}
	const int num_hook = sizeof(hook) / sizeof(hook[0]);
	for (int i = 0; i < num_hook; i++) 
	{
		Signature sig = hook[i].sig;
		void* result = (void*)SigScan(serverbin, sig.data, sig.mask, error, maxlen);
		if (!result)
		{
			return false;
		}
		*hook[i].result = result;
		if (hook[i].detour) {
			*hook[i].detourHook = subhook_new((void*)result, hook[i].hookFunction, SUBHOOK_64BIT_OFFSET);
			subhook_install(*hook[i].detourHook);
		}
	}

	return true;
}

internal void Hooks_UnhookFunctions()
{
	SH_REMOVE_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_REMOVE_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientCommand, gameclients, SH_STATIC(Hook_ClientCommand), false);

	const int num_hook = sizeof(hook) / sizeof(hook[0]);
	for (int i = 0; i < num_hook; i++) {
		if (hook[i].detourHook != NULL) {
			subhook_remove(*hook[i].detourHook);
			subhook_free(*hook[i].detourHook);
		}
	}
}