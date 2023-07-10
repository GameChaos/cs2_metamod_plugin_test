
SH_DECL_HOOK3_void(ISource2Server, GameFrame, SH_NOATTRIB, false, bool, bool, bool);
SH_DECL_HOOK1_void(ISource2GameClients, ClientFullyConnect, SH_NOATTRIB, false, CPlayerSlot);
SH_DECL_HOOK5(ISource2GameClients, ProcessUsercmds, SH_NOATTRIB, false, float, CPlayerSlot, bf_read *, int, bool, bool);

typedef CBasePlayerController *PlayerSlotToPlayerController_t(CPlayerSlot slot);
PlayerSlotToPlayerController_t *PlayerSlotToPlayerController = NULL;

typedef void CEntityInstance_entindex_t(CEntityInstance *this_, CEntityIndex *out);
CEntityInstance_entindex_t *CEntityInstance_entindex = NULL;

#define CBASEPLAYERPAWN_POSTTHINK(name) void name(CBasePlayerPawn *this_)
typedef CBASEPLAYERPAWN_POSTTHINK(CCSPP_PostThink_t);
CCSPP_PostThink_t *CCSPP_PostThink = NULL;
subhook_t CCSPP_PostThink_hook;
internal CBASEPLAYERPAWN_POSTTHINK(Hook_CCSPP_PostThink);

#define CCSP_MS__CHECKJUMPBUTTON(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__CHECKJUMPBUTTON(CCSP_MS__CheckJumpButton_t);
CCSP_MS__CheckJumpButton_t *CCSP_MS__CheckJumpButton = NULL;
subhook_t CCSP_MS__CheckJumpButton_hook;
internal CCSP_MS__CHECKJUMPBUTTON(Hook_CCSP_MS__CheckJumpButton);

#define CCSP_MS__WALKMOVE(name) void name(CCSPlayer_MovementServices *this_, CMoveData *mv)
typedef CCSP_MS__WALKMOVE(CCSP_MS__WalkMove_t);
CCSP_MS__WalkMove_t *CCSP_MS__WalkMove = NULL;
subhook_t CCSP_MS__WalkMove_hook;
internal CCSP_MS__WALKMOVE(Hook_CCSP_MS__WalkMove);

#define CREATEENTITY(name) CBaseEntity *name(void *this_, u32 a2, void *class_, void *memory, s32 zero, u32 a6, u32 a7, bool a8)
typedef CREATEENTITY(CreateEntity_t);
CreateEntity_t *CreateEntity = NULL;
subhook_t CreateEntity_hook;
internal CREATEENTITY(Hook_CreateEntity);

internal bool Hooks_HookFunctions(char *error, size_t maxlen)
{
	SH_ADD_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_ADD_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_ADD_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	
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
	
	// Hook_CreateEntity
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

internal void Hooks_UnhookFunctions()
{
	SH_REMOVE_HOOK(ISource2Server, GameFrame, gamedll, &Hook_GameFrame, false);
	SH_REMOVE_HOOK(ISource2GameClients, ClientFullyConnect, gameclients, SH_STATIC(Hook_ClientFullyConnect), false);
	SH_REMOVE_HOOK(ISource2GameClients, ProcessUsercmds, gameclients, SH_STATIC(Hook_ProcessUsercmds), false);
	
	subhook_remove(CCSPP_PostThink_hook);
	subhook_free(CCSPP_PostThink_hook);
	
	subhook_remove(CCSP_MS__CheckJumpButton_hook);
	subhook_free(CCSP_MS__CheckJumpButton_hook);
	
	subhook_remove(CCSP_MS__WalkMove_hook);
	subhook_free(CCSP_MS__WalkMove_hook);
	
	subhook_remove(CreateEntity_hook);
	subhook_free(CreateEntity_hook);
}