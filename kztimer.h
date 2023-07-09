#define PRE_VELMOD_MAX 1.104 // Calculated 276/250
#define PRE_VELMOD_INCREMENT 0.1152     // per second. (0.0009 per tick on 128 tick)
#define PRE_VELMOD_INCREMENT_FAST 0.128 // per second. (0.0001 per tick on 128 tick)
#define PRE_VELMOD_DECREMENT 0.576      // per second. (0.0045 per tick on 128 tick)
#define PRE_VELMOD_DECREMENT_FAST 5.12  // per second. (0.04 per tick on 128 tick)
#define PRE_COUNTER_MAX 0.5859375       // in seconds? (75 frames on 128 tick)
#include "mathlib/vector.h"

f32 gfPrespeed = 0.0f;
f32 gfMaxspeed = 250.0f;
float g_PrestrafeVelocity = 1.0;
float g_RealVelPreMod = 1.0;
float g_fVelocityModifierLastChange;
float g_PrestrafeFrameCounter;
QAngle oldEyeAngles;

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

internal b32 IsButtonDown(CInButtonState buttons, InputBitMask_t button)
{
	b32 result = buttons.m_pButtonStates[0] & button;
	return result;
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

float GetClientMovingDirection(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	Vector fVelocity = mv->m_vecVelocity;
	QAngle fEyeAngles = mv->m_vecViewAngles;

	if (fEyeAngles.x > 70.0) fEyeAngles.x = 70.0;
	if (fEyeAngles.x < -70.0) fEyeAngles.x = -70.0;

	Vector viewDirection;
	if (ms->pawn->m_MoveType == MOVETYPE_LADDER)
	{
		viewDirection = ms->m_vecLadderNormal;
	}
	else
	{
		AngleVectors(mv->m_vecViewAngles, &viewDirection);
	}
	VectorNormalize(fVelocity);
	VectorNormalize(viewDirection);
	float direction = DotProduct(fVelocity, viewDirection);
	if (ms->pawn->m_MoveType == MOVETYPE_LADDER)
		direction = direction * -1;
	return direction;
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

float CalcPrestrafeVelMod(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	if (!(ms->pawn->m_fFlags & FL_ONGROUND))
	{
		return g_PrestrafeVelocity;
	}

	//DoPrintCenter(NULL, "Moveright %i | Turnright %i\nMoveleft %i | Turnleft %i",
	//	!!IsButtonDown(ms->m_nButtons, IN_MOVERIGHT), !!IsTurningRight(ms, mv),
	//	!!IsButtonDown(ms->m_nButtons, IN_MOVELEFT), !!IsTurningLeft(ms, mv));
	if (!IsTurning(ms, mv))
	{
		if (gpGlobals->curtime - g_fVelocityModifierLastChange > 0.2)
		{
			g_PrestrafeVelocity = 1.0;
			g_fVelocityModifierLastChange = gpGlobals->curtime;
		}
		else if (g_PrestrafeVelocity > PRE_VELMOD_MAX + 0.007)
		{
			return PRE_VELMOD_MAX - 0.001; // Returning without setting the variable is intentional
		}
	}
	else if ((IsButtonDown(ms->m_nButtons, IN_MOVELEFT) || IsButtonDown(ms->m_nButtons, IN_MOVERIGHT)) && mv->m_vecVelocity.Length2D() > 248.9)
	{
		float increment = PRE_VELMOD_INCREMENT * gpGlobals->interval_per_tick;
		if (g_PrestrafeVelocity > 1.04)
		{
			increment = PRE_VELMOD_INCREMENT_FAST * gpGlobals->interval_per_tick;
		}

		bool forwards = GetClientMovingDirection(ms, mv) > 0.0;

		if ((IsButtonDown(ms->m_nButtons, IN_MOVERIGHT) && IsTurningRight(ms, mv)) || (IsTurningLeft(ms, mv) && !forwards)
			|| (IsButtonDown(ms->m_nButtons, IN_MOVELEFT) && IsTurningLeft(ms, mv)) || (IsTurningRight(ms, mv) && !forwards))
		{
			g_PrestrafeFrameCounter++;

			if (float(g_PrestrafeFrameCounter) * gpGlobals->interval_per_tick < PRE_COUNTER_MAX)
			{
				g_PrestrafeVelocity += increment;
				if (g_PrestrafeVelocity > PRE_VELMOD_MAX)
				{
					if (g_PrestrafeVelocity > PRE_VELMOD_MAX + 0.007)
					{
						g_PrestrafeVelocity = PRE_VELMOD_MAX - 0.001;
					}
					else
					{
						g_PrestrafeVelocity -= 0.007;
					}
				}
				g_PrestrafeVelocity += increment;
			}
			else
			{
				g_PrestrafeVelocity -= PRE_VELMOD_DECREMENT * gpGlobals->interval_per_tick;
				g_PrestrafeFrameCounter -= 2;

				if (g_PrestrafeVelocity < 1.0)
				{
					g_PrestrafeVelocity = 1.0;
					g_PrestrafeFrameCounter = 0;
				}
			}
		}
		else
		{
			g_PrestrafeVelocity -= PRE_VELMOD_DECREMENT_FAST * gpGlobals->interval_per_tick;

			if (g_PrestrafeVelocity < 1.0)
			{
				g_PrestrafeVelocity = 1.0;
			}
		}

		g_fVelocityModifierLastChange = gpGlobals->curtime;
	}
	else
	{
		g_PrestrafeFrameCounter = 0;
		return 1.0; // Returning without setting the variable is intentional
	}

	return g_PrestrafeVelocity;
}