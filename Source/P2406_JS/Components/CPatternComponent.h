#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Patterns/CPatternStructures.h"
#include "CPatternComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDecidedPattern, bool, InValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDecidedPattern_Range, float, InValue);

USTRUCT(BlueprintType)
struct FPatternInfo
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 PatternID; // ���� ���� ID

	UPROPERTY(EditAnywhere)
	int32 Phase = 0;

	UPROPERTY(EditAnywhere)
	float Priority = 0.0f;

	UPROPERTY(EditAnywhere)
	float ActionRange = 0.0f;

	UPROPERTY(EditAnywhere)
	float Cooldown = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<int32> ConditionIDs; // ������ ������ �� �ִ� ���� ID ����Ʈ

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<class UCActiveSkill*> ActiveSkills; // �� ������ ������ ��ų��


public:
	float CurrentCooldown = 0.0f;

public:
	FPatternInfo()
		: PatternID(-1), Phase(0), Priority(0.0f), ActionRange(0.0f)
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
	FORCEINLINE bool IsExecutePattern() const {return bExecutePattern;}

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
	void Begin_Pattern();
	void End_Pattern();

public:
	FDecidedPattern OnDecidedPattern;
	FDecidedPattern_Range OnDecidedPattern_Range;

private:
	class ACharacter* OwnerCharacter;



private:
	int32 CurrentPhase;
	int32 PatternIndex;

private:
	TArray<FPatternInfo> PatternInfos; 
	bool bDecided = false;
	bool bExecutePattern = false;
	FPatternInfo* DecidedPattern;

private:
	UDataTable* PatternDataTable;
	class UCGameInstance* GameInstance;
	class UCPatternConditionManager* PatternCondition;
};
