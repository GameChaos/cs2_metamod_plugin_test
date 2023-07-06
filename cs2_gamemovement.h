
#ifndef CS2_GAMEMOVEMENT_H
#define CS2_GAMEMOVEMENT_H

#include <stdint.h>

// Size: 0x20
class CInButtonState
{
public:
	uint8_t __pad0000[0x8]; // 0x0
	uint64_t m_pButtonStates[3]; // 0x8
};
static_assert(sizeof(CInButtonState) == 0x20, "Class didn't match expected size");

// Size: 0x40
class CPlayerPawnComponent
{
public:
	uint8_t __pad0000[0x8]; // 0x0
	// MNetworkDisable
	// MNetworkChangeAccessorFieldPathIndex
	CNetworkVarChainer __m_pChainEntity; // 0x8
	uint8_t __pad0030[0x10]; // 0x0
};
static_assert(sizeof(CPlayerPawnComponent) == 0x40, "Class didn't match expected size");

// Size: 0x1d0
class CPlayer_MovementServices : public CPlayerPawnComponent
{
public:
	int32_t m_nImpulse; // 0x40
	uint8_t __pad0044[0x4]; // 0x44
	CInButtonState m_nButtons; // 0x48
	uint64_t m_nQueuedButtonDownMask; // 0x68
	uint64_t m_nQueuedButtonChangeMask; // 0x70
	uint64_t m_nButtonDoublePressed; // 0x78
	uint32_t m_pButtonPressedCmdNumber[64]; // 0x80
	uint32_t m_nLastCommandNumberProcessed; // 0x180
	uint8_t __pad0184[0x4]; // 0x184
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	uint64_t m_nToggleButtonDownMask; // 0x188
	// MNetworkEnable
	// MNetworkBitCount "12"
	// MNetworkMinValue "0.000000"
	// MNetworkMaxValue "2048.000000"
	// MNetworkEncodeFlags
	float m_flMaxspeed; // 0x190
	// MNetworkEnable
	float m_arrForceSubtickMoveWhen[4]; // 0x194
	float m_flForwardMove; // 0x1a4
	float m_flLeftMove; // 0x1a8
	float m_flUpMove; // 0x1ac
	Vector m_vecLastMovementImpulses; // 0x1b0
	QAngle m_vecOldViewAngles; // 0x1bc
	uint8_t __pad01c8[0x8]; // 0x44
};
static_assert(sizeof(CPlayer_MovementServices) == 0x1d0, "Class didn't match expected size");

// Size: 0x220
class CPlayer_MovementServices_Humanoid : public CPlayer_MovementServices
{
public:
	float m_flStepSoundTime; // 0x1d0	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	// MNetworkBitCount "17"
	// MNetworkMinValue "-4096.000000"
	// MNetworkMaxValue "4096.000000"
	// MNetworkEncodeFlags
	float m_flFallVelocity; // 0x1d4	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	bool m_bInCrouch; // 0x1d8	
	uint8_t __pad01d9[0x3]; // 0x1d9
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	uint32_t m_nCrouchState; // 0x1dc	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	GameTime_t m_flCrouchTransitionStartTime; // 0x1e0	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	bool m_bDucked; // 0x1e4	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	bool m_bDucking; // 0x1e5	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	bool m_bInDuckJump; // 0x1e6	
	uint8_t __pad01e7[0x1]; // 0x1e7
	Vector m_groundNormal; // 0x1e8	
	float m_flSurfaceFriction; // 0x1f4	
	CUtlStringToken m_surfaceProps; // 0x1f8	
	uint8_t __pad01fc[0xc]; // 0x1fc
	int32_t m_nStepside; // 0x208	
	int32_t m_iTargetVolume; // 0x20c	
	Vector m_vecSmoothedVelocity; // 0x210	
};
static_assert(sizeof(CPlayer_MovementServices_Humanoid) == 0x220, "Class didn't match expected size");

// Size: 0x13f0
class CCSPlayer_MovementServices : public CPlayer_MovementServices_Humanoid
{
public:
	// MNetworkEnable
	float m_flMaxFallVelocity; // 0x220	
	// MNetworkEnable
	// MNetworkEncoder
	Vector m_vecLadderNormal; // 0x224	
	// MNetworkEnable
	int32_t m_nLadderSurfacePropIndex; // 0x230	
	// MNetworkEnable
	float m_flDuckAmount; // 0x234	
	// MNetworkEnable
	float m_flDuckSpeed; // 0x238	
	// MNetworkEnable
	bool m_bDuckOverride; // 0x23c	
	// MNetworkEnable
	bool m_bDesiresDuck; // 0x23d	
	uint8_t __pad023e[0x2]; // 0x23e
	float m_flDuckOffset; // 0x240	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	// MNetworkPriority "32"
	uint32_t m_nDuckTimeMsecs; // 0x244	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	uint32_t m_nDuckJumpTimeMsecs; // 0x248	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	uint32_t m_nJumpTimeMsecs; // 0x24c	
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	float m_flLastDuckTime; // 0x250	
	uint8_t __pad0254[0xc]; // 0x254
	Vector2D m_vecLastPositionAtFullCrouchSpeed; // 0x260	
	bool m_duckUntilOnGround; // 0x268	
	bool m_bHasWalkMovedSinceLastJump; // 0x269	
	bool m_bInStuckTest; // 0x26a	
	uint8_t __pad026b[0x5]; // 0x26b
	uint64_t m_CachedGetPointContents[3][64]; // 0x270	
	Vector m_CachedGetPointContentsPoint[3][64]; // 0x870	
	uint8_t __pad1170[0x8]; // 0x1170
	float m_flStuckCheckTime[2][64]; // 0x1178	
	int32_t m_nTraceCount; // 0x1378	
	int32_t m_StuckLast; // 0x137c	
	bool m_bSpeedCropped; // 0x1380	
	uint8_t __pad1381[0x3]; // 0x1381
	int32_t m_nOldWaterLevel; // 0x1384	
	float m_flWaterEntryTime; // 0x1388	
	Vector m_vecForward; // 0x138c	
	Vector m_vecLeft; // 0x1398	
	Vector m_vecUp; // 0x13a4	
	Vector m_vecPreviouslyPredictedOrigin; // 0x13b0	
	bool m_bMadeFootstepNoise; // 0x13bc	
	uint8_t __pad13bd[0x3]; // 0x13bd
	int32_t m_iFootsteps; // 0x13c0	
	int32_t m_nRoundFootstepsMade; // 0x13c4	
	// MNetworkEnable
	bool m_bOldJumpPressed; // 0x13c8	
	uint8_t __pad13c9[0x3]; // 0x13c9
	float m_flJumpPressedTime; // 0x13cc	
	// MNetworkEnable
	float m_flJumpUntil; // 0x13d0	
	// MNetworkEnable
	float m_flJumpVel; // 0x13d4	
	// MNetworkEnable
	GameTime_t m_fStashGrenadeParameterWhen; // 0x13d8	
	uint8_t __pad13dc[0x4]; // 0x13dc
	// MNetworkEnable
	uint64_t m_nButtonDownMaskPrev; // 0x13e0	
	// MNetworkEnable
	float m_flOffsetTickCompleteTime; // 0x13e8	
	// MNetworkEnable
	float m_flOffsetTickStashedSpeed; // 0x13ec	
};
static_assert(sizeof(CCSPlayer_MovementServices) == 0x13f0, "Class didn't match expected size");

// Size: 0xE0
class CMoveData
{
public:
	// TODO: fix offsets that were fricked due to game updates.
	uint8_t moveDataFlags; // 0x0
	CHandle<CBaseEntity> m_nPlayerHandle; // 0x4 don't know if this is actually a CHandle. <CBaseEntity> is a placeholder
	uint8_t unknown1[0xC];
	Vector m_vecViewAngles; // 0x14
	float m_flForwardMove; // 0x20	
	float m_flSideMove; // 0x24
	float m_flUpMove; // 0x28
	uint8_t unknown2[0x10]; // 0x2c
	Vector m_vecVelocity; // 0x30
	Vector m_vecAngles; // 0x3c
	uint8_t unknown3[0x38];
	// these 2 vecs are used in FinishMove -> Unk21(this, ucmd, mv) -> void __fastcall sub_7FF8E0960610(CMoveData *mv)
	Vector m_unknownVec0; // 0x80
	Vector m_unknownVec1; // 0x8c
	Vector m_vecAbsOrigin; // 0x98
	uint8_t unknown5[0x4]; // 0xa4
	Vector m_vecTrailingVelocity; // 0xa8
	float m_flTrailingVelocityTime; // 0xb4
	uint8_t unknown6[0x4]; // 0xb8
	Vector m_vecOldAngles; // 0xbc
	float m_flMaxSpeed; // 0xc8
	float m_flClientMaxSpeed; // 0xcc
	bool m_bJumpedThisTick; // 0xd0 related to dev_cs_enable_consistent_jumps
	bool m_bSomethingWithGravity; // 0xd1 related to the new ShouldApplyGravity
	uint8_t unknown7[0x2]; // 0xd2 Probably padding
	Vector m_outWishVel; // 0xd4
};

#endif // CS2_GAMEMOVEMENT_H