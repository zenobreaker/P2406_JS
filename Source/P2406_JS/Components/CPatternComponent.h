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
	int32 PatternID; // 패턴 고유 ID

	UPROPERTY(EditAnywhere)
	int32 Phase = 0;

	UPROPERTY(EditAnywhere)
	float Priority = 0.0f;

	UPROPERTY(EditAnywhere)
	float ActionRange = 0.0f;

	UPROPERTY(EditAnywhere)
	TArray<int32> ConditionIDs; // 패턴을 수행할 수 있는 조건 ID 리스트

	UPROPERTY(EditAnywhere)
	class UCActiveSkill* ActiveSkill; // 이 패턴이 실행할 스킬


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
	UPROPERTY(EditAnywhere, Category = "Pattern")
	int32 MaxPhase;

	UPROPERTY(EditAnywhere, Category = "Pattern")
	TArray<FPatternData> PatternDatas;

private:
	UPROPERTY(EditAnywhere, Category = "Pattern")
	TSubclassOf<class UCPatternDecider> PatternDeciderClass;

public:
	FORCEINLINE bool GetDecidePattern() const { return bDecided; }
	FORCEINLINE bool IsExecutePattern() const { return bExecutePattern; }
	FORCEINLINE int32 GetSelectedPatternID() const { return SelectedPatternID; }
	FORCEINLINE class UCActiveSkill* GetCurrentActiveSkill() { return CurrentSkill; }
public:
	UCPatternComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ExecutePattern();
	void DecidePattern();
public:
	void Begin_Pattern();
	void End_Pattern();


public:
	void OnActivated_Collision(FName InName = NAME_None);
	void OnDeactivated_Collision(FName InName = NAME_None);


private: 
	UFUNCTION()
	void UpdatedPhase(int32 InPhase);

public:
	FDecidedPattern OnDecidedPattern;
	FDecidedPattern_Range OnDecidedPattern_Range;

private:
	class ACharacter* OwnerCharacter;

private:
	int32 CurrentPhase;
	int32 PatternIndex;

private:
	UPROPERTY()
	TMap<int32, FPatternInfo> PatternInfos;

	UPROPERTY()
	class UCActiveSkill* CurrentSkill;

private:
	bool bDecided = false;
	bool bExecutePattern = false;
	int32 SelectedPatternID; 

private:
	UDataTable* PatternDataTable;
	class UCGameInstance* GameInstance;
	class UCPatternConditionManager* PatternCondition;
	class UCPatternDecider* PatternDecider; 
};
