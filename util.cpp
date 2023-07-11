f32 gfPrespeed = 0.0f;
QAngle oldEyeAngles;

void UpdateOldEyeAngles(CMoveData* mv)
{
	oldEyeAngles = mv->m_vecViewAngles;
}

b32 IsTurning(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	return oldEyeAngles.y != mv->m_vecViewAngles.y;
}

b32 IsTurningLeft(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	return IsTurning(ms, mv) && (mv->m_vecViewAngles.y < oldEyeAngles.y - 180
		|| mv->m_vecViewAngles.y > oldEyeAngles.y && mv->m_vecViewAngles.y < oldEyeAngles.y + 180);
}
b32 IsTurningRight(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	return IsTurning(ms, mv) && !IsTurningLeft(ms, mv);
}

b32 IsButtonDown(CInButtonState buttons, InputBitMask_t button)
{
	b32 result = buttons.m_pButtonStates[0] & button;
	return result;
}

char* GetSpeedText(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	char buffer[128];
	if (ms->pawn->m_fFlags & FL_ONGROUND)
	{
		sprintf(buffer, "Speed: %.0f", mv->m_vecVelocity.Length2D());
	}
	else
	{
		sprintf(buffer, "Speed: %.0f (%.0f)", mv->m_vecVelocity.Length2D(), gfPrespeed);
	}
	return buffer;
}

char* GetKeyText(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	char buffer[128];
	sprintf(buffer, "Keys: %s %s %s %s %s %s",
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
int GetPlayerIndex(CBasePlayerPawn* const pawn)
{
	int result = -1;
	if (pawn->m_hController.m_Index != 0xffffffff)
	{
		result = pawn->m_hController.m_Index & 0x3fff;
	}
	return result;
}

int GetPlayerIndex(CPlayer_MovementServices* const ms)
{
	return GetPlayerIndex(ms->pawn);
}

int GetPlayerIndex(CMoveData* const mv)
{
	return GetPlayerIndex(static_cast<CBasePlayerPawn*>(CSource2EntitySystem__EntityByIndex(gpEntitySystem, mv->m_nPlayerHandle.m_Index & 0x3fff)));
}

int GetPlayerIndex(CBasePlayerController* const pc)
{
	CEntityIndex result = -1;
	CEntityInstance_entindex(pc, &result);
	return result.Get();
}

CBasePlayerController* GetPawnController(CBasePlayerPawn* const pawn)
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
