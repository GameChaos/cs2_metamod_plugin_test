#define PS_VELMOD_MAX 1.104 // Calculated 276/250
#define PS_INCREMENT (0.0009f * 128.0f)
#define PS_INCREMENT_FAST (0.001f * 128.0f)
#define PS_DECREMENT (0.007f * 128.0f)
#define PS_DECREMENT_FAST (0.04f * 128.0f)
#define PS_MAX_COUNT (75.0f / 128.0f)
#include "mathlib/vector.h"

f32 g_PrestrafeVelocity = 1.0;
f32 g_RealVelPreMod = 1.0;
f32 g_fVelocityModifierLastChange;
f32 g_PrestrafeFrameCounter;


f32 GetClientMovingDirection(CCSPlayer_MovementServices* ms, CMoveData* mv)
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

	f32 direction = DotProduct(fVelocity, viewDirection);

	if (ms->pawn->m_MoveType == MOVETYPE_LADDER)
		direction = direction * -1;
	return direction;
}


f32 GetClientMovingDirection(CCSPlayer_MovementServices *ms, CMoveData *mv, bool ladder)
{
	QAngle angles = mv->m_vecViewAngles;
	angles[0] = CLAMP(-70.0f, angles[0], 70.0f);
	
	Vector viewNormal;
	if (ladder)
	{
		viewNormal = ms->m_vecLadderNormal;
	}
	else
	{
		AngleVectors(mv->m_vecViewAngles, &viewNormal, NULL, NULL);
	}
	
	Vector velocity = mv->m_vecVelocity.Normalized();
	
	f32 direction = velocity.Dot(viewNormal);
	if (ladder)
	{
		direction *= -1;
	}
	return direction;
}


float CalcPrestrafeVelMod(PlayerData *pd, CCSPlayer_MovementServices *ms, CMoveData *mv)
{
	if (!(ms->pawn->m_fFlags & FL_ONGROUND))
	{
		return pd->preVelMod;
	}
	float speed = mv->m_vecVelocity.Length2D();
	
	if (!pd->turning)
	{
		if (gpGlobals->curtime - pd->preVelModLastChange > 0.2f)
		{
			pd->preVelMod = 1.0f;
			pd->preVelModLastChange = gpGlobals->curtime;
		}
		else if (pd->preVelMod > PS_VELMOD_MAX + 0.007f)
		{
			return PS_VELMOD_MAX - 0.001f; // Returning without setting the variable is intentional
		}
	}
	else if ((IsButtonDown(ms->m_nButtons, IN_MOVERIGHT) || IsButtonDown(ms->m_nButtons, IN_MOVELEFT)) && speed > 248.9)
	{
		float increment = PS_INCREMENT * gpGlobals->interval_per_tick;
		if (pd->preVelMod > 1.04f)
		{
			increment = PS_INCREMENT_FAST * gpGlobals->interval_per_tick;;
		}
		
		bool forwards = GetClientMovingDirection(ms, mv, false) > 0.0f;
		
		if ((IsButtonDown(ms->m_nButtons, IN_MOVERIGHT) && pd->turning == TURN_RIGHT || pd->turning == TURN_LEFT && !forwards)
			 || (IsButtonDown(ms->m_nButtons, IN_MOVELEFT) && pd->turning == TURN_LEFT || pd->turning == TURN_RIGHT && !forwards))
		{
			pd->preCounter += gpGlobals->interval_per_tick;
			
			if (pd->preCounter < PS_MAX_COUNT)
			{
				pd->preVelMod += increment;
				if (pd->preVelMod > PS_VELMOD_MAX)
				{
					if (pd->preVelMod > PS_VELMOD_MAX + 0.007f)
					{
						pd->preVelMod = PS_VELMOD_MAX - 0.001f;
					}
					else
					{
						pd->preVelMod -= PS_DECREMENT * gpGlobals->interval_per_tick;
					}
				}
				pd->preVelMod += increment;
			}
			else
			{
				pd->preVelMod -= PS_DECREMENT * 0.5f * gpGlobals->interval_per_tick;
				pd->preCounter -= gpGlobals->interval_per_tick * 2.0f;
				
				if (pd->preVelMod < 1.0)
				{
					pd->preVelMod = 1.0;
					pd->preCounter = 0;
				}
			}
		}
		else
		{
			pd->preVelMod -= PS_DECREMENT_FAST * gpGlobals->interval_per_tick;
			
			if (pd->preVelMod < 1.0)
			{
				pd->preVelMod = 1.0;
			}
		}
		
		pd->preVelModLastChange = gpGlobals->curtime;
	}
	else
	{
		pd->preCounter = 0;
		return 1.0; // Returning without setting the variable is intentional
	}
	
	return pd->preVelMod;
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