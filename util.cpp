#define MAXPLAYERS 64

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

// Returns the player slot / entindex corresponding to the object.
int GetPlayerIndex(CBasePlayerPawn *const pawn)
{
	int result = -1;
	if (pawn->m_hController.m_Index != 0xffffffff)
	{
		result = pawn->m_hController.m_Index & 0x3fff;
	}
	return result;
}

int GetPlayerIndex(CPlayer_MovementServices *const ms)
{
	return GetPlayerIndex(ms->pawn);
}

int GetPlayerIndex(CMoveData *const mv)
{
	return GetPlayerIndex(static_cast<CBasePlayerPawn*>(CGameEntitySystem__EntityByIndex(g_entitySystem, mv->m_nPlayerHandle.m_Index & 0x3fff)));
}

int GetPlayerIndex(CBasePlayerController *const pc)
{
	CEntityIndex result = -1;
	CEntityInstance_entindex(pc, &result);
	return result.Get();
}

CBasePlayerController* GetPawnController(CBasePlayerPawn *const pawn)
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
