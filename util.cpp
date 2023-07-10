f32 gfPrespeed = 0.0f;

QAngle oldEyeAngles;

void UpdateOldEyeAngles(CMoveData* mv)
{
	oldEyeAngles = mv->m_vecViewAngles;
}

float IsTurning(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	return oldEyeAngles.y != mv->m_vecViewAngles.y;
}

float IsTurningLeft(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	return IsTurning(ms, mv) && (mv->m_vecViewAngles.y < oldEyeAngles.y - 180
		|| mv->m_vecViewAngles.y > oldEyeAngles.y && mv->m_vecViewAngles.y < oldEyeAngles.y + 180);
}
float IsTurningRight(CCSPlayer_MovementServices* ms, CMoveData* mv)
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
