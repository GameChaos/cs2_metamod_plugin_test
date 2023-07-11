#define PRE_VELMOD_MAX 1.104 // Calculated 276/250
#define PRE_VELMOD_INCREMENT 0.1152     // per second. (0.0009 per tick on 128 tick)
#define PRE_VELMOD_INCREMENT_FAST 0.128 // per second. (0.0001 per tick on 128 tick)
#define PRE_VELMOD_DECREMENT 0.576      // per second. (0.0045 per tick on 128 tick)
#define PRE_VELMOD_DECREMENT_FAST 5.12  // per second. (0.04 per tick on 128 tick)
#define PRE_COUNTER_MAX 0.5859375       // in seconds? (75 frames on 128 tick)
#include "mathlib/vector.h"

f32 g_PrestrafeVelocity = 1.0;
f32 g_RealVelPreMod = 1.0;
f32 g_fVelocityModifierLastChange;
f32 g_PrestrafeFrameCounter;

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


float CalcPrestrafeVelMod(CCSPlayer_MovementServices* ms, CMoveData* mv)
{
	if (!(ms->pawn->m_fFlags & FL_ONGROUND))
	{
		return g_PrestrafeVelocity;
	}

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

void SetupKZTimerConvars()
{
	engine->ServerCommand("sv_accelerate 6.5");
	engine->ServerCommand("sv_accelerate_use_weapon_speed 0");
	engine->ServerCommand("sv_airaccelerate 100");
	engine->ServerCommand("sv_enablebunnyhopping 1");
	engine->ServerCommand("sv_friction 5");
	engine->ServerCommand("sv_ladder_scale_speed 1");
	engine->ServerCommand("sv_maxvelocity 2000");
	engine->ServerCommand("sv_staminamax 0");
	engine->ServerCommand("sv_wateraccelerate 10");
	engine->ServerCommand("sv_jump_spam_penalty_time 0.0078125");	
}