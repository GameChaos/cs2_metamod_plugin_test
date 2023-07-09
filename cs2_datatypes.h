
#ifndef CS2_DATATYPES_H
#define CS2_DATATYPES_H

typedef uint32_t WorldGroupId_t;
typedef int32_t GameTick_t;
typedef float GameTime_t;
typedef uint64_t CNetworkedQuantizedFloat;

class CBaseFilter;

#include "../game/shared/ehandle.h"
#include "utlsymbollarge.h"
#include "ihandleentity.h"
#include "vscript/ivscript.h"
//#include "../game/server/recipientfilter.h"

enum TakeDamageFlags_t : uint32_t
{
	DFLAG_NONE = 0x0,
	DFLAG_SUPPRESS_HEALTH_CHANGES = 0x1,
	DFLAG_SUPPRESS_PHYSICS_FORCE = 0x2,
	DFLAG_SUPPRESS_EFFECTS = 0x4,
	DFLAG_PREVENT_DEATH = 0x8,
	DFLAG_FORCE_DEATH = 0x10,
	DFLAG_ALWAYS_GIB = 0x20,
	DFLAG_NEVER_GIB = 0x40,
	DFLAG_REMOVE_NO_RAGDOLL = 0x80,
	DFLAG_SUPPRESS_DAMAGE_MODIFICATION = 0x100,
	DFLAG_ALWAYS_FIRE_DAMAGE_EVENTS = 0x200,
	DFLAG_RADIUS_DMG = 0x400,
	DMG_LASTDFLAG = 0x400,
	DFLAG_IGNORE_ARMOR = 0x800,
};

enum SurroundingBoundsType_t : uint8_t
{
	USE_OBB_COLLISION_BOUNDS = 0x0,
	USE_BEST_COLLISION_BOUNDS = 0x1,
	USE_HITBOXES = 0x2,
	USE_SPECIFIED_BOUNDS = 0x3,
	USE_GAME_CODE = 0x4,
	USE_ROTATION_EXPANDED_BOUNDS = 0x5,
	USE_COLLISION_BOUNDS_NEVER_VPHYSICS = 0x6,
	USE_ROTATION_EXPANDED_SEQUENCE_BOUNDS = 0x7,
	SURROUNDING_TYPE_BIT_COUNT = 0x3,
};

enum EInButtonState : uint32_t
{
	IN_BUTTON_UP = 0x0,
	IN_BUTTON_DOWN = 0x1,
	IN_BUTTON_DOWN_UP = 0x2,
	IN_BUTTON_UP_DOWN = 0x3,
	IN_BUTTON_UP_DOWN_UP = 0x4,
	IN_BUTTON_DOWN_UP_DOWN = 0x5,
	IN_BUTTON_DOWN_UP_DOWN_UP = 0x6,
	IN_BUTTON_UP_DOWN_UP_DOWN = 0x7,
	IN_BUTTON_STATE_COUNT = 0x8,
};

enum InputBitMask_t : uint64_t
{
	IN_NONE = 0x0,
	IN_ALL = 0xffffffffffffffff,
	IN_ATTACK = 0x1,
	IN_JUMP = 0x2,
	IN_DUCK = 0x4,
	IN_FORWARD = 0x8,
	IN_BACK = 0x10,
	IN_USE = 0x20,
	IN_TURNLEFT = 0x80,
	IN_TURNRIGHT = 0x100,
	IN_MOVELEFT = 0x200,
	IN_MOVERIGHT = 0x400,
	IN_ATTACK2 = 0x800,
	IN_RELOAD = 0x2000,
	IN_SPEED = 0x10000,
	IN_JOYAUTOSPRINT = 0x20000,
	IN_FIRST_MOD_SPECIFIC_BIT = 0x100000000,
	IN_USEORRELOAD = 0x100000000,
	IN_SCORE = 0x200000000,
	IN_ZOOM = 0x400000000,
	IN_LOOK_AT_WEAPON = 0x800000000,
};

// Size: 0x2
struct ChangeAccessorFieldPathIndex_t
{
	int16_t m_Value; // 0x0
};

// NOTE: Stub!
class CEntityHandle
{
public:
	uint32_t m_Value;
};

// Size: 0x4
class CUtlStringToken
{
public:
	uint32_t m_nHashCode;
};
static_assert(sizeof(CUtlStringToken) == 4, "Class didn't match expected size");

// Size: 0x30
class CNetworkOriginCellCoordQuantizedVector
{
public:
	uint8_t __pad0000[0x10]; // 0x0
	// MNetworkEnable
	// MNetworkChangeCallback "OnCellChanged"
	// MNetworkPriority "31"
	// MNetworkSerializer "cellx"
	uint16_t m_cellX; // 0x10
	// MNetworkEnable
	// MNetworkChangeCallback "OnCellChanged"
	// MNetworkPriority "31"
	// MNetworkSerializer "celly"
	uint16_t m_cellY; // 0x12
	// MNetworkEnable
	// MNetworkChangeCallback "OnCellChanged"
	// MNetworkPriority "31"
	// MNetworkSerializer "cellz"
	uint16_t m_cellZ; // 0x14
	// MNetworkEnable
	uint16_t m_nOutsideWorld; // 0x16
	// MNetworkBitCount "15"
	// MNetworkMinValue "0.000000"
	// MNetworkMaxValue "1024.000000"
	// MNetworkEncodeFlags
	// MNetworkChangeCallback "OnCellChanged"
	// MNetworkPriority "31"
	// MNetworkSerializer "posx"
	CNetworkedQuantizedFloat m_vecX; // 0x18
	// MNetworkBitCount "15"
	// MNetworkMinValue "0.000000"
	// MNetworkMaxValue "1024.000000"
	// MNetworkEncodeFlags
	// MNetworkChangeCallback "OnCellChanged"
	// MNetworkPriority "31"
	// MNetworkSerializer "posy"
	CNetworkedQuantizedFloat m_vecY; // 0x20
	// MNetworkBitCount "15"
	// MNetworkMinValue "0.000000"
	// MNetworkMaxValue "1024.000000"
	// MNetworkEncodeFlags
	// MNetworkChangeCallback "OnCellChanged"
	// MNetworkPriority "31"
	// MNetworkSerializer "posz"
	CNetworkedQuantizedFloat m_vecZ; // 0x28
};
static_assert(sizeof(CNetworkOriginCellCoordQuantizedVector) == 0x30, "Class didn't match expected size");

// Size: 0x28
class CNetworkVarChainer
{
public:
	uint8_t __pad0000[0x20]; // 0x0
	// MNetworkDisable
	// MNetworkChangeAccessorFieldPathIndex
	ChangeAccessorFieldPathIndex_t m_PathIndex; // 0x20
	uint8_t __pad0022[0x6];
};
static_assert(sizeof(CNetworkVarChainer) == 0x28, "Class didn't match expected size");

// Size: 0x78
class CEntityIdentity
{
public:
	uint8_t __pad0000[0x14]; // 0x0
	// MNetworkEnable
	// MNetworkChangeCallback "entityIdentityNameChanged"
	int32_t m_nameStringableIndex; // 0x14
	CUtlSymbolLarge m_name; // 0x18
	CUtlSymbolLarge m_designerName; // 0x20
	uint8_t __pad0028[0x8]; // 0x28
	uint32_t m_flags; // 0x30
	uint8_t __pad0034[0x4]; // 0x34
	// MNetworkDisable
	WorldGroupId_t m_worldGroupId; // 0x38
	uint32_t m_fDataObjectTypes; // 0x3c
	// MNetworkDisable
	// MNetworkChangeAccessorFieldPathIndex
	ChangeAccessorFieldPathIndex_t m_PathIndex; // 0x40
	uint8_t __pad0042[0x16]; // 0x42
	CEntityIdentity *m_pPrev; // 0x58
	CEntityIdentity *m_pNext; // 0x60
	CEntityIdentity *m_pPrevByClass; // 0x68
	CEntityIdentity *m_pNextByClass; // 0x70
};
static_assert(sizeof(CEntityIdentity) == 0x78, "Class didn't match expected size");

// Alignment: 0
// Size: 0x8
class CEntityComponent
{
public:
	uint8_t __pad0000[0x8]; // 0x0
	// No members available
};
static_assert(sizeof(CEntityComponent) == 8, "Class didn't match expected size");

// Size: 0x38
class CScriptComponent : public CEntityComponent
{
public:
	uint8_t __pad0008[0x28]; // 0x8
	CUtlSymbolLarge m_scriptClassName; // 0x30
};
static_assert(sizeof(CScriptComponent) == 0x38, "Class didn't match expected size");

// Size: 0x30
class CEntityInstance : public IHandleEntity
{
public:
	// MNetworkDisable
	CUtlSymbolLarge m_iszPrivateVScripts; // 0x8
	// MNetworkEnable
	// MNetworkPriority "56"
	CEntityIdentity *m_pEntity; // 0x10
	uint8_t __pad0018[0x10]; // 0x18
	// MNetworkEnable
	// MNetworkDisable
	CScriptComponent *m_CScriptComponent; // 0x28
};
static_assert(sizeof(CEntityInstance) == 0x30, "Class didn't match expected size");

// Size: 0x20
class CTransform
{
public:
	Vector m_vPosition;
private:
	uint8_t __padding[4];
public:
	Vector4D m_orientation;
};
static_assert(sizeof(CTransform) == 0x20, "Class didn't match expected size");

// Alignment: 2
// Size: 0x10
class CGameSceneNodeHandle
{
private:
	[[maybe_unused]] uint8_t __pad0000[0x8]; // 0x0
public:
	// MNetworkEnable
	CEntityHandle m_hOwner; // 0x8
	// MNetworkEnable
	CUtlStringToken m_name; // 0xc
};
static_assert(sizeof(CGameSceneNodeHandle) == 0x10, "Class didn't match expected size");

// Size: 0x150
class CGameSceneNode
{
public:
	uint8_t __pad0000[0x10]; // 0x0
	// MNetworkDisable
	CTransform m_nodeToWorld; // 0x10
	// MNetworkDisable
	CEntityInstance *m_pOwner; // 0x30
	// MNetworkDisable
	CGameSceneNode *m_pParent; // 0x38
	// MNetworkDisable
	CGameSceneNode *m_pChild; // 0x40
	// MNetworkDisable
	CGameSceneNode *m_pNextSibling; // 0x48
	uint8_t __pad0050[0x20]; // 0x50
	// MNetworkEnable
	// MNetworkSerializer "gameSceneNode"
	// MNetworkChangeCallback "gameSceneNodeHierarchyParentChanged"
	// MNetworkPriority "32"
	// MNetworkVarEmbeddedFieldOffsetDelta "8"
	CGameSceneNodeHandle m_hParent; // 0x70
	// MNetworkEnable
	// MNetworkPriority "32"
	// MNetworkUserGroup "Origin"
	// MNetworkChangeCallback "gameSceneNodeLocalOriginChanged"
	CNetworkOriginCellCoordQuantizedVector m_vecOrigin; // 0x80
	uint8_t __pad00b0[0x8]; // 0xb0
	// MNetworkEnable
	// MNetworkEncoder
	// MNetworkPriority "32"
	// MNetworkSerializer "gameSceneNodeStepSimulationAnglesSerializer"
	// MNetworkChangeCallback "gameSceneNodeLocalAnglesChanged"
	QAngle m_angRotation; // 0xb8
	// MNetworkEnable
	// MNetworkChangeCallback "gameSceneNodeLocalScaleChanged"
	// MNetworkPriority "32"
	float m_flScale; // 0xc4
	// MNetworkDisable
	Vector m_vecAbsOrigin; // 0xc8
	// MNetworkDisable
	QAngle m_angAbsRotation; // 0xd4
	// MNetworkDisable
	float m_flAbsScale; // 0xe0
	// MNetworkDisable
	int16_t m_nParentAttachmentOrBone; // 0xe4
	// MNetworkDisable
	bool m_bDebugAbsOriginChanges; // 0xe6
	// MNetworkDisable
	bool m_bDormant; // 0xe7
	// MNetworkDisable
	bool m_bForceParentToBeNetworked; // 0xe8
	struct
	{
		// MNetworkDisable
		uint8_t m_bDirtyHierarchy: 1;
		// MNetworkDisable
		uint8_t m_bDirtyBoneMergeInfo: 1;
		// MNetworkDisable
		uint8_t m_bNetworkedPositionChanged: 1;
		// MNetworkDisable
		uint8_t m_bNetworkedAnglesChanged: 1;
		// MNetworkDisable
		uint8_t m_bNetworkedScaleChanged: 1;
		// MNetworkDisable
		uint8_t m_bWillBeCallingPostDataUpdate: 1;
		// MNetworkDisable
		uint8_t m_bNotifyBoneTransformsChanged: 1;
		// MNetworkDisable
		uint8_t m_bBoneMergeFlex: 1;
		// MNetworkDisable
		uint8_t m_nLatchAbsOrigin: 2;
		// MNetworkDisable
		uint8_t m_bDirtyBoneMergeBoneToRoot: 1;
	}; // 24 bits
	// MNetworkDisable
	uint8_t m_nHierarchicalDepth; // 0xeb
	// MNetworkDisable
	uint8_t m_nHierarchyType; // 0xec
	// MNetworkDisable
	uint8_t m_nDoNotSetAnimTimeInInvalidatePhysicsCount; // 0xed
	uint8_t __pad00ee[0x2]; // 0xee
	// MNetworkEnable
	CUtlStringToken m_name; // 0xf0
	uint8_t __pad00f4[0x3c]; // 0xf4
	// MNetworkEnable
	// MNetworkChangeCallback "gameSceneNodeHierarchyAttachmentChanged"
	CUtlStringToken m_hierarchyAttachName; // 0x130
	// MNetworkDisable
	float m_flZOffset; // 0x134
	// MNetworkDisable
	Vector m_vRenderOrigin; // 0x138
	uint8_t __pad0144[8];
};
static_assert(sizeof(CGameSceneNode) == 0x150, "Class didn't match expected size");

// Alignment: 2
// Size: 0x50
class CBodyComponent : public CEntityComponent
{
public:
	// MNetworkDisable
	CGameSceneNode *m_pSceneNode; // 0x8
	uint8_t __pad0010[0x10]; // 0x10
	// MNetworkDisable
	// MNetworkChangeAccessorFieldPathIndex
	CNetworkVarChainer __m_pChainEntity; // 0x20
	uint8_t __pad0028[0x8];
};
static_assert(sizeof(CBodyComponent) == 0x50, "Class didn't match expected size");

// Size: 0x1b0
class CNetworkTransmitComponent
{
public:
	uint8_t __pad0000[0x16c]; // 0x0
	uint8_t m_nTransmitStateOwnedCounter; // 0x16c
	uint8_t __pad016d[0x43]; // 0x0
};
static_assert(sizeof(CNetworkTransmitComponent) == 0x1b0, "Class didn't match expected size");

// Size: 0x20
struct thinkfunc_t
{
	uint8_t __pad0000[0x8]; // 0x0
	HSCRIPT m_hFn; // 0x8
	CUtlStringToken m_nContext; // 0x10
	GameTick_t m_nNextThinkTick; // 0x14
	GameTick_t m_nLastThinkTick; // 0x18
};
static_assert(sizeof(thinkfunc_t) == 0x20, "Class didn't match expected size");

// Size: 0x18
struct ResponseContext_t
{
	CUtlSymbolLarge m_iszName; // 0x0
	CUtlSymbolLarge m_iszValue; // 0x8
	GameTime_t m_fExpirationTime; // 0x10
};
static_assert(sizeof(ResponseContext_t) == 0x18, "Class didn't match expected size");

// Size: 0x28
class CEntityIOOutput
{
public:
	uint8_t __pad0000[0x18]; // 0x0
	// TODO: CVariantBase< CVariantDefaultAllocator >
	uint8_t m_Value[0x10]; // 0x18 CVariantBase<CVariantDefaultAllocator>
};
static_assert(sizeof(CEntityIOOutput) == 0x28, "Class didn't match expected size");

// Size: 0x28
class CNetworkVelocityVector
{
public:
	uint8_t __pad0000[0x10]; // 0x0
	// MNetworkBitCount "18"
	// MNetworkMinValue "-4096.000000"
	// MNetworkMaxValue "4096.000000"
	// MNetworkEncodeFlags
	// MNetworkChangeCallback "CNetworkVelocityVector"
	CNetworkedQuantizedFloat m_vecX; // 0x10
	// MNetworkBitCount "18"
	// MNetworkMinValue "-4096.000000"
	// MNetworkMaxValue "4096.000000"
	// MNetworkEncodeFlags
	// MNetworkChangeCallback "CNetworkVelocityVector"
	CNetworkedQuantizedFloat m_vecY; // 0x18
	// MNetworkBitCount "18"
	// MNetworkMinValue "-4096.000000"
	// MNetworkMaxValue "4096.000000"
	// MNetworkEncodeFlags
	// MNetworkChangeCallback "CNetworkVelocityVector"
	CNetworkedQuantizedFloat m_vecZ; // 0x20
};
static_assert(sizeof(CNetworkVelocityVector) == 0x28, "Class didn't match expected size");

// Size: 0x30
struct VPhysicsCollisionAttribute_t
{
public:
	uint8_t __pad0000[0x8]; // 0x0
	// MNetworkEnable
	uint64_t m_nInteractsAs; // 0x8
	// MNetworkEnable
	uint64_t m_nInteractsWith; // 0x10
	// MNetworkEnable
	uint64_t m_nInteractsExclude; // 0x18
	// MNetworkEnable
	uint32_t m_nEntityId; // 0x20
	// MNetworkEnable
	uint32_t m_nOwnerId; // 0x24
	// MNetworkEnable
	uint16_t m_nHierarchyId; // 0x28
	// MNetworkEnable
	uint8_t m_nCollisionGroup; // 0x2a
	// MNetworkEnable
	uint8_t m_nCollisionFunctionMask; // 0x2b
};

static_assert(sizeof(VPhysicsCollisionAttribute_t) == 0x30, "Class didn't match expected size");

// Size: 0xb0
class CCollisionProperty
{
public:
	uint8_t __pad0000[0x10]; // 0x0
	// MNetworkEnable
	// MNetworkChangeCallback "CollisionAttributeChanged"
	VPhysicsCollisionAttribute_t m_collisionAttribute; // 0x10
	// MNetworkEnable
	// MNetworkChangeCallback "OnUpdateOBB"
	Vector m_vecMins; // 0x40
	// MNetworkEnable
	// MNetworkChangeCallback "OnUpdateOBB"
	Vector m_vecMaxs; // 0x4c
	uint8_t __pad0058[0x2]; // 0x58
	// MNetworkEnable
	// MNetworkChangeCallback "OnUpdateSolidFlags"
	uint8_t m_usSolidFlags; // 0x5a
	// MNetworkEnable
	// MNetworkChangeCallback "OnUpdateSolidType"
	SolidType_t m_nSolidType; // 0x5b
	// MNetworkEnable
	// MNetworkChangeCallback "MarkSurroundingBoundsDirty"
	uint8_t m_triggerBloat; // 0x5c
	// MNetworkEnable
	// MNetworkChangeCallback "MarkSurroundingBoundsDirty"
	SurroundingBoundsType_t m_nSurroundType; // 0x5d
	// MNetworkEnable
	uint8_t m_CollisionGroup; // 0x5e
	// MNetworkEnable
	// MNetworkChangeCallback "OnUpdateEnablePhysics"
	uint8_t m_nEnablePhysics; // 0x5f
	float m_flBoundingRadius; // 0x60
	// MNetworkEnable
	// MNetworkChangeCallback "MarkSurroundingBoundsDirty"
	Vector m_vecSpecifiedSurroundingMins; // 0x64
	// MNetworkEnable
	// MNetworkChangeCallback "MarkSurroundingBoundsDirty"
	Vector m_vecSpecifiedSurroundingMaxs; // 0x70
	Vector m_vecSurroundingMaxs; // 0x7c
	Vector m_vecSurroundingMins; // 0x88
	// MNetworkEnable
	Vector m_vCapsuleCenter1; // 0x94
	// MNetworkEnable
	Vector m_vCapsuleCenter2; // 0xa0
	// MNetworkEnable
	float m_flCapsuleRadius; // 0xac
};
static_assert(sizeof(CCollisionProperty) == 0xb0, "Class didn't match expected size");

// Size: 0x4b0
class CBaseEntity : public CEntityInstance
{
public:
	// MNetworkEnable
	// MNetworkUserGroup "CBodyComponent"
	// MNetworkAlias "CBodyComponent"
	// MNetworkTypeAlias "CBodyComponent"
	// MNetworkPriority "48"
	CBodyComponent *m_CBodyComponent; // 0x30
	CNetworkTransmitComponent m_NetworkTransmitComponent; // 0x38
	uint8_t __pad01e8[0x40]; // 0x1e8
	CUtlVector< thinkfunc_t > m_aThinkFunctions; // 0x228
	int32_t m_iCurrentThinkContext; // 0x240
	GameTick_t m_nLastThinkTick; // 0x244
	uint8_t __pad0248[0x8]; // 0x248
	CBitVec< 64 > m_isSteadyState; // 0x250
	float m_lastNetworkChange; // 0x258
	uint8_t __pad025c[0xc]; // 0x25c
	CUtlVector< ResponseContext_t > m_ResponseContexts; // 0x268
	CUtlSymbolLarge m_iszResponseContext; // 0x280
	uint8_t __pad0288[0x20]; // 0x288
	// MNetworkEnable
	// MNetworkSerializer "ClampHealth"
	// MNetworkUserGroup "Player"
	// MNetworkPriority "32"
	int32_t m_iHealth; // 0x2a8
	// MNetworkEnable
	int32_t m_iMaxHealth; // 0x2ac
	// MNetworkEnable
	// MNetworkUserGroup "Player"
	// MNetworkPriority "32"
	uint8_t m_lifeState; // 0x2b0
	uint8_t __pad02b1[0x3]; // 0x2b1
	float m_flDamageAccumulator; // 0x2b4
	// MNetworkEnable
	bool m_bTakesDamage; // 0x2b8
	uint8_t __pad02b9[0x3]; // 0x2b9
	// MNetworkEnable
	TakeDamageFlags_t m_nTakeDamageFlags; // 0x2bc
	uint8_t __pad02c0[0x1]; // 0x2c0
	// MNetworkEnable
	MoveCollide_t m_MoveCollide; // 0x2c1
	// MNetworkEnable
	MoveType_t m_MoveType; // 0x2c2
	uint8_t m_nWaterTouch; // 0x2c3
	uint8_t m_nSlimeTouch; // 0x2c4
	bool m_bRestoreInHierarchy; // 0x2c5
	uint8_t __pad02c6[0x2]; // 0x2c6
	CUtlSymbolLarge m_target; // 0x2c8
	float m_flMoveDoneTime; // 0x2d0
	CHandle< CBaseFilter > m_hDamageFilter; // 0x2d4
	CUtlSymbolLarge m_iszDamageFilterName; // 0x2d8
	// MNetworkEnable
	// MNetworkSendProxyRecipientsFilter
	CUtlStringToken m_nSubclassID; // 0x2e0
	uint8_t __pad02e4[0xc]; // 0x2e4
	// MNetworkEnable
	// MNetworkPriority "0"
	// MNetworkSerializer "animTimeSerializer"
	// MNetworkSendProxyRecipientsFilter
	float m_flAnimTime; // 0x2f0
	// MNetworkEnable
	// MNetworkPriority "1"
	// MNetworkSerializer "simulationTimeSerializer"
	// MNetworkSendProxyRecipientsFilter
	float m_flSimulationTime; // 0x2f4
	// MNetworkEnable
	GameTime_t m_flCreateTime; // 0x2f8
	// MNetworkEnable
	bool m_bClientSideRagdoll; // 0x2fc
	// MNetworkEnable
	uint8_t m_ubInterpolationFrame; // 0x2fd
	uint8_t __pad02fe[0x2]; // 0x2fe
	Vector m_vPrevVPhysicsUpdatePos; // 0x300
	// MNetworkEnable
	uint8_t m_iTeamNum; // 0x30c
	uint8_t __pad030d[0x3]; // 0x30d
	CUtlSymbolLarge m_iGlobalname; // 0x310
	int32_t m_iSentToClients; // 0x318
	float m_flSpeed; // 0x31c
	CUtlString m_sUniqueHammerID; // 0x320
	// MNetworkEnable
	uint32_t m_spawnflags; // 0x328
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	GameTick_t m_nNextThinkTick; // 0x32c
	int32_t m_nSimulationTick; // 0x330
	uint8_t __pad0334[0x4]; // 0x334
	CEntityIOOutput m_OnKilled; // 0x338
	// MNetworkEnable
	// MNetworkPriority "32"
	// MNetworkUserGroup "Player"
	uint32_t m_fFlags; // 0x360
	Vector m_vecAbsVelocity; // 0x364
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	// MNetworkPriority "32"
	CNetworkVelocityVector m_vecVelocity; // 0x370
	uint8_t __pad0398[0x8]; // 0x398
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	Vector m_vecBaseVelocity; // 0x3a0
	int32_t m_nPushEnumCount; // 0x3ac
	CCollisionProperty *m_pCollision; // 0x3b0
	// MNetworkEnable
	CHandle< CBaseEntity > m_hEffectEntity; // 0x3b8
	// MNetworkEnable
	// MNetworkPriority "32"
	CHandle< CBaseEntity > m_hOwnerEntity; // 0x3bc
	// MNetworkEnable
	// MNetworkChangeCallback "OnEffectsChanged"
	uint32_t m_fEffects; // 0x3c0
	// MNetworkEnable
	// MNetworkPriority "32"
	// MNetworkUserGroup "Player"
	CHandle< CBaseEntity > m_hGroundEntity; // 0x3c4
	// MNetworkEnable
	// MNetworkBitCount "8"
	// MNetworkMinValue "0.000000"
	// MNetworkMaxValue "4.000000"
	// MNetworkEncodeFlags
	// MNetworkUserGroup "LocalPlayerExclusive"
	float m_flFriction; // 0x3c8
	// MNetworkEnable
	// MNetworkEncoder
	float m_flElasticity; // 0x3cc
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	float m_flGravityScale; // 0x3d0
	// MNetworkEnable
	// MNetworkUserGroup "LocalPlayerExclusive"
	float m_flTimeScale; // 0x3d4
	// MNetworkEnable
	// MNetworkUserGroup "Water"
	// MNetworkBitCount "8"
	// MNetworkMinValue "0.000000"
	// MNetworkMaxValue "1.000000"
	// MNetworkEncodeFlags
	float m_flWaterLevel; // 0x3d8
	// MNetworkEnable
	bool m_bSimulatedEveryTick; // 0x3dc
	// MNetworkEnable
	bool m_bAnimatedEveryTick; // 0x3dd
	bool m_bDisableLowViolence; // 0x3de
	uint8_t m_nWaterType; // 0x3df
	int32_t m_iEFlags; // 0x3e0
	uint8_t __pad03e4[0x4]; // 0x3e4
	CEntityIOOutput m_OnUser1; // 0x3e8
	CEntityIOOutput m_OnUser2; // 0x410
	CEntityIOOutput m_OnUser3; // 0x438
	CEntityIOOutput m_OnUser4; // 0x460
	int32_t m_iInitialTeamNum; // 0x488
	// MNetworkEnable
	GameTime_t m_flNavIgnoreUntilTime; // 0x48c
	QAngle m_vecAngVelocity; // 0x490
	bool m_bNetworkQuantizeOriginAndAngles; // 0x49c
	bool m_bLagCompensate; // 0x49d
	uint8_t __pad049e[0x2]; // 0x49e
	float m_flOverriddenFriction; // 0x4a0
	CHandle< CBaseEntity > m_pBlocker; // 0x4a4
	float m_flLocalTime; // 0x4a8
	float m_flVPhysicsUpdateLocalTime; // 0x4ac
};
static_assert(sizeof(CBaseEntity) == 0x4b0, "Class didn't match expected size");

#endif // CS2_DATATYPES_H