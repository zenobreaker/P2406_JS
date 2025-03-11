#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CBossPhaseData.generated.h"

UENUM(BlueprintType)
enum class EPhaseTransitionType : uint8
{
	HPThreshold UMETA(DisplayName = "HP 기반"),
	TimeElapsed UMETA(DisplayName = "시간 기반"),
	AttackTrigger UMETA(DisplayName = "특정 공격 사용 후"),
	CustomCondition UMETA(DisplayName = "커스텀 조건"),
	Max,
};


USTRUCT(BlueprintType)
struct FBossPhase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Transition")
	EPhaseTransitionType TransitionType = EPhaseTransitionType::Max;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Transition")
	float HPThreshold = -1.0f; // 체력 조건 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Transition")
	float TimeThreshold = -1.0f; // 시간 지남 조건 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Transition")
	FString RequireAttackName; // 특정 공격 후 전환 

	// 블루프린트 커스텀 조건이라는데 이건 나중에 생각하자 
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Transition")
	bool bCumstomCondition = false; */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Transition")
	int32 ValidPhase = 0; // 예: 1이면 1페이즈에서만 적용
};

UCLASS()
class P2406_JS_API UCBossPhaseData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Data")
	int32 BossMaxPhase = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Phase Data")
	TArray<FBossPhase> PhaseConditions; // 여러 개의 페이즈 조건 저장 가능 
};
