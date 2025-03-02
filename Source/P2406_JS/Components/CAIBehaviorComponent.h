#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/FEnum.h"
#include "CAIBehaviorComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);

//UENUM()
//enum class EWaitPatternState : uint8
//{
//	IdleWait = 0, CircleAvoid, Max
//};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	FORCEINLINE bool GetCanMove() const { return bCanMove; }

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

	FORCEINLINE UBlackboardComponent* GetBlackboard() const { return Blackboard; }
	
	FORCEINLINE int32 GetMyToken() const { return Token; }
	FORCEINLINE void SetMyToken(int32 InValue) { Token= InValue; }

private:
	UPROPERTY(EditAnywhere, Category = "Key")
	FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatrolLocationKey = "PatrolLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName EqsLocationKey = "EqsLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatternDecideKey = "PatternDecide";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatternNumber = "PatternNumber";

private:
	EAIStateType GetType();

public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsDamageMode();
	bool IsAvoidMode();
	bool IsAirborneMode();
	bool IsDownMode();
	bool IsGuardMode();
	bool IsDeadMode();

public:
	UCAIBehaviorComponent();

protected:
	virtual void BeginPlay() override;

public:
	class ACharacter* GetTarget();
	void SetTarget(class ACharacter* InTarget);

	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);

	FVector GetEqsLocation();
	void SetEqsLocation(const FVector& InLocation);

	bool GetPattrenDecide(); 
	void SetPatternDecide(bool InPatternDecide);

	int32 GetPatternNumber();
	void SetPatternNumber(int32 InPatternNumber);

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetDamageMode();
	void SetAvoidMode();
	void SetAirborneMode();
	void SetDownMode();
	void SetGuardMode();
	void SetDeadMode();
	void SetNoneMode();


private:
	FString EnumToString(EAIStateType InType);

	void ChangeType(EAIStateType InType);

public:
	UFUNCTION()
	void OnStateChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnAddCondiitionType(EConditionState InType);

	UFUNCTION()
	void OnRemoveConditionType(EConditionState InType);

	UFUNCTION()
	void OnCharacterDead();

	UFUNCTION()
	void OnDecidedPattern(bool InValue);

public:
	FAIStateTypeChanged OnAIStateTypeChanged;



private:
	class UBlackboardComponent* Blackboard;

private:
	class ACEnemy_AI* CachedAI;

	bool bCanMove = true;
	
	EAIStateType PrevType; 

	int32 Token = -1;
};
