#define MAXPLAYERS 64

internal void UnlockCvars()
{
	static b32 alreadyUnlocked;
	if (alreadyUnlocked) return;

	{
		ConVarHandle handle;
		handle.Set(0);

		ConVar* cvar = g_pCVar->GetConVar(handle);
		while (cvar != nullptr)
		{
			cvar->flags = cvar->flags & ~FCVAR_DEVELOPMENTONLY;
			cvar->flags = cvar->flags & ~FCVAR_HIDDEN;
			cvar->flags = cvar->flags & ~FCVAR_PROTECTED;
			handle.Set(handle.Get() + 1);
			if (g_pCVar->GetConVar(handle) == nullptr) break;
			cvar = g_pCVar->GetConVar(handle);
		}
	}

	{
		ConCommandHandle handle;
		handle.Set(0);
		ConCommand *cmd = g_pCVar->GetCommand(handle);
		while (cmd != nullptr)
		{
			cmd->RemoveFlags(FCVAR_DEVELOPMENTONLY | FCVAR_HIDDEN | FCVAR_PROTECTED);
			if (!strcmp(cmd->GetName(), "playvol"))
			{
				cmd->AddFlags(FCVAR_SERVER_CAN_EXECUTE);
			}
			handle.Set(handle.Get() + 1);
			if (g_pCVar->GetCommand(handle) == nullptr || !strcmp(cmd->GetName(), "<unknown>")) break;
			cmd = g_pCVar->GetCommand(handle);
		}
	}
	
	alreadyUnlocked = true;
}

internal b32 IsValidPlayerSlot(s32 slot)
{
	b32 result = slot > 0 && slot <= MAXPLAYERS;
	return result;
}

internal b32 IsValidPlayerSlot(CPlayerSlot slot)
{
	b32 result = slot.Get() > 0 && slot.Get() <= MAXPLAYERS;
	return result;
}

TurnState GetTurning(PlayerData* pd, CMoveData* mv)
{
	bool turning = pd->oldAngles.y != mv->m_vecViewAngles.y;
	if (!turning) return TURN_NONE;
	if (mv->m_vecViewAngles.y < pd->oldAngles.y - 180
		|| mv->m_vecViewAngles.y > pd->oldAngles.y && mv->m_vecViewAngles.y < pd->oldAngles.y + 180) return TURN_LEFT;
	return TURN_RIGHT;
}

b32 IsButtonDown(CInButtonState buttons, InputBitMask_t button, bool checkStillPressed = false)
{
	b32 result = buttons.m_pButtonStates[0] & button;

	if (!checkStillPressed)
	{
		result |= buttons.m_pButtonStates[1] & button || buttons.m_pButtonStates[2] & button;
	}
	return result;
}

char* GetTimerText(PlayerData* pd)
{
	char buffer[48];
	if (pd->timerRunning)
	{
		float time = gpGlobals->curtime - pd->timerStartTime;
		int roundedTime = floor(time * 100); // Time rounded to number of centiseconds

		int centiseconds = roundedTime % 100;
		roundedTime = (roundedTime - centiseconds) / 100;
		int seconds = roundedTime % 60;
		roundedTime = (roundedTime - seconds) / 60;
		int minutes = roundedTime % 60;
		roundedTime = (roundedTime - minutes) / 60;
		int hours = roundedTime;

		if (hours == 0)
		{
			snprintf(buffer, sizeof(buffer), "Time: %02i:%02i.%02i", minutes, seconds, centiseconds);
		}
		else
		{
			snprintf(buffer, sizeof(buffer), "Time: %i:%02i:%02i.%02i", hours, minutes, seconds, centiseconds);
		}
		return buffer;
	}
	else
	{
		snprintf(buffer, sizeof(buffer), "Time: Stopped");
	}
	return buffer;
}

char* GetSpeedText(PlayerData *pd, CCSPlayer_MovementServices *ms, CMoveData *mv)
{
	char buffer[128];
	if (ms->pawn->m_fFlags & FL_ONGROUND)
	{
		snprintf(buffer, sizeof(buffer), "Speed: %.0f", mv->m_vecVelocity.Length2D());
	}
	else
	{
		snprintf(buffer, sizeof(buffer), "Speed: %.0f (%.0f)", mv->m_vecVelocity.Length2D(), pd->preSpeed);
	}
	return buffer;
}

char* GetKeyText(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	char buffer[128];
	snprintf(buffer, sizeof(buffer), "Keys: %s %s %s %s %s %s",
		IsButtonDown(ms->m_nButtons, IN_MOVELEFT) ? "A" : "_",
		IsButtonDown(ms->m_nButtons, IN_FORWARD) ? "W" : "_",
		IsButtonDown(ms->m_nButtons, IN_BACK) ? "S" : "_",
		IsButtonDown(ms->m_nButtons, IN_MOVERIGHT) ? "D" : "_",
		IsButtonDown(ms->m_nButtons, IN_DUCK) ? "C" : "_",
		IsButtonDown(ms->m_nButtons, IN_JUMP) ? "J" : "_");
	return buffer;
}

void DoPrintCenter(const char* fmt, ...)
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
	PrintCenterTextToAll(buffer);
}

void DoPrintChat(const char* fmt, ...)
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
	PrintChatToAll(buffer);
}

// Returns the player slot / entindex corresponding to the object.
int GetPlayerIndex(CBasePlayerPawn *pawn)
{
	int result = -1;
	if (pawn->m_hController.m_Index != 0xffffffff)
	{
		result = pawn->m_hController.m_Index & 0x3fff;
	}
	return result;
}

int GetPlayerIndex(CPlayer_UseServices* us)
{
	return GetPlayerIndex(us->pawn);
}

int GetPlayerIndex(CPlayer_MovementServices *ms)
{
	return GetPlayerIndex(ms->pawn);
}

int GetPlayerIndex(CMoveData *mv)
{
	return GetPlayerIndex(static_cast<CBasePlayerPawn*>(CGameEntitySystem__EntityByIndex(g_entitySystem, mv->m_nPlayerHandle.m_Index & 0x3fff)));
}

int GetPlayerIndex(CBasePlayerController *pc)
{
	CEntityIndex result = -1;
	CEntityInstance_entindex(pc, &result);
	return result.Get();
}

CBasePlayerController* GetPawnController(CBasePlayerPawn *pawn)
{
	CBasePlayerController* result = NULL;
	CEntityIndex index = GetPlayerIndex(pawn);
	// TODO: what is maxplayers?
	if (index.Get() > 0)
	{
		result = PlayerSlotToPlayerController(CPlayerSlot(index.Get() - 1));
	}
	return result;
}

void LoadCheckpoint(CCSPlayerPawn *pawn, PlayerData *pd)
{
	CPlayerSlot slot = GetPlayerIndex(pawn);
	Checkpoint cp = pd->checkpoints[pd->checkpoints.Count() - 1];
	pd->lastOrigin = cp.origin;
	pd->lastAngle = cp.angles;
	pd->teleportTime = gpGlobals->curtime;
	Vector newVelocity;
	newVelocity.Init();
	CBaseAnimGraph__Teleport(pawn, &cp.origin, &cp.angles, &newVelocity);
	if (cp.groundEnt.m_Index != 0xffffffff)
	{
		pawn->m_hGroundEntity = cp.groundEnt;
	}
	if (cp.onLadder)
	{
		static_cast<CCSPlayer_MovementServices*>(pawn->m_pMovementServices)->m_vecLadderNormal = cp.ladderNormal;
		pawn->m_MoveType = MOVETYPE_LADDER;
	}
}