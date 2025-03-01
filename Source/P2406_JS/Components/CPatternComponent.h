#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Patterns/CPatternStructures.h"
#include "CPatternComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDecidedPattern, bool, InValue);

USTRUCT(BlueprintType)
struct FPatternInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PatternID; // 패턴 고유 ID

	UPROPERTY(EditAnywhere)
	int32 Phase = 0;

	UPROPERTY(EditAnywhere)
	float Priority = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ConditionIDs; // 패턴을 수행할 수 있는 조건 ID 리스트

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<class UCActiveSkill*> ActiveSkills; // 이 패턴이 실행할 스킬들

public:
	FPatternInfo()
		: PatternID(-1), Phase(0), Priority(0.0f)
    {}
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCPatternComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	int32 MaxPhase;
	
	UPROPERTY(EditAnywhere)
	TArray<FPatternData> PatternDatas;

public:
	FORCEINLINE bool GetDecidePattern() const { return bDecided; }

public:
	UCPatternComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetPattern(int32 InPaternID);

	void ExecutePattern();

	void DecidePattern();

public:
	FDecidedPattern OnDecidedPattern;


private:
	class ACharacter* OwnerCharacter;



private:
	int32 CurrentPhase;
	int32 PatternIndex;

private:
	TArray<FPatternInfo> PatternInfos; 
	bool bDecided = false;
	FPatternInfo* DecidedPattern;

private:
	UDataTable* PatternDataTable;
	class UCGameInstance* GameInstance;
	class UCPatternConditionManager* PatternCondition;
};
