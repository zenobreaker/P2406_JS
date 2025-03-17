#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "CPatternConditionManager.generated.h"


UENUM(BlueprintType)
enum class EPatternConditionType : uint8
{
    HealthBelow,    // 특정 수치 이하 
    TimeElapsed,    // 일정 시간 경과
    PlayerInRange,  // 거리 
    BossDamagedRatio,    // 일정 데미지 
    Max,
};

USTRUCT(BlueprintType)
struct FPatternConditionInfo : public FTableRowBase
{
    GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 ID = 0;

    UPROPERTY(EditAnywhere)
	EPatternConditionType  Type = EPatternConditionType::Max;

	UPROPERTY(EditAnywhere)
	float Value = 0.0f; 
};



class FPatternCondition
{
public:
	FPatternCondition(EPatternConditionType  InType)
		: ConditionType(InType)
		{}
	FPatternCondition(EPatternConditionType  InType, float InValue)
		: ConditionType(InType)
		, Value(InValue)
	{}


	bool IsConditionMet(class ACharacter* InCharacter);

private:
	EPatternConditionType  ConditionType;
	float Value;
};

UCLASS()
class P2406_JS_API UCPatternConditionManager : public UObject
{
	GENERATED_BODY()

public: 
	UCPatternConditionManager();

public:
	void InitConditionData();


	void AddCondition(int32 InConditionID, FString TriggerType, float InValue);
	void AddCondition(int32 InConditionID, EPatternConditionType InConditionType, float InValue);
	bool CheckCondition(int32 InConditionID, class ACharacter* InCharacter);


private:
	TMap<int32, TUniquePtr<FPatternCondition>> PatternConditionTable;

private:
	UDataTable* DataTable = nullptr;
};
