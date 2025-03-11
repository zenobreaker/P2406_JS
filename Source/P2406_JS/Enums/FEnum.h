#pragma once

#include "CoreMinimal.h"

///////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Fist, Sword, Hammer, Warp, Bow, Max,
};

///////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EStateType : uint8
{
	Idle = 0, 
	Evade ,
	Dash, 
	Equip, 
	Damaged, 
	Action,
	Airborne, 
	Down, 
	Guard,
	Dead, 
	Execute,

	Max, 
};


///////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, 
	Approach,
	Action, 
	Patrol, 
	Damage, 
	Avoid, 
	Airborne, 
	Down, 
	Guard, 
	Pattern_Decide,
	Strafe,
	Dead,

	Max,
};


///////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EConditionState : uint8
{
    CONDITION_NONE = 0 UMETA(DisplayName = "None"),
    CONDITION_DOWNED = 1 << 0 UMETA(DisplayName = "Downed"),
    CONDITION_STRUNED = 1 << 1 UMETA(DisplayName = "Stunned"),
    CONDITION_FROZEN = 1 << 2 UMETA(DisplayName = "Frozen"),
    CONDITION_AIRBORNE = 1 << 3 UMETA(DisplayName = "Airborne"),
    CONDITION_SUPERARMOR = 1 << 3 UMETA(DisplayName = "Airborne"),
    // 필요 상태 추가
};


///////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EEntityGrade : uint8
{
	None = 0, 
	Normal ,
	Elite,
	Boss, 

	MAX,
};

///////////////////////////////////////////////////////////////////////////////
UENUM(BlueprintType)
enum class EParryType : uint8
{
	None,
	Special,
	MAX,
};

///////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class ESkillPhase : uint8
{
	Start = 0,
	Begin_Casting,
	End_Casting,
	Begin_Skill,
	End_Skill,
	Create_Effect,
	Create_Collision,
	Finished,

	ExtraPhase		= 10,
	Begin_Charging,			// 추가: 충전 중
	End_Charging,
	WaitingForInput ,	// 추가: 입력 대기 중

	Max = Finished + 1,
};

///////////////////////////////////////////////////////////////////////////////

UENUM(BlueprintType)
enum class ESkillCollisionType : uint8
{
	Sphere,
	Box,
	Capsule,
	Max,
};

///////////////////////////////////////////////////////////////////////////////

