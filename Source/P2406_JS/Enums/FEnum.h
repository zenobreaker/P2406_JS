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