#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/FEnum.h"
#include "CAIBehaviorComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotated, bool, InValue);

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
	FORCEINLINE void SetMyToken(int32 InValue) { Token = InValue; }

private:
	UPROPERTY(EditAnywhere, Category = "Key")
	FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName LateTargetKey = "LateTarget";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatrolLocationKey = "PatrolLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName EqsLocationKey = "EqsLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatternDecideKey = "bPatternDecide";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatternExecuteKey = "bPatternExecute";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName DecidePatternNumber = "CurrentPattern";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName ActionRangeKey = "ActionRange";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName StrafeKey = "StrafeMode";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName CurrentPhaseKey = "CurrentPhase";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName LastPhaseKey = "LastPhase";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName IsRotatingKey = "bIsRotating";


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
	class ACharacter* GetLateTarget();
	void SetLateTarget(class ACharacter* InTarget);

	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);

	FVector GetEqsLocation();
	void SetEqsLocation(const FVector& InLocation);

	bool GetPattrenDecide();
	void SetPatternDecide(bool InPatternDecide);

	bool GetPatternExecute();
	void SetPatternExecute(bool InValue);
	void SetDecidePatternNumber(int32 InValue);
	float GetActionRange();
	void SetActionRange(float InActionRange);

	void SetStrafe(bool InValue);
	bool GetStrafe();

	void SetLastPhase(int32 InPhase);
	int32 GetLastPhase();

	void SetCurrentPhase(int32 InPhase);
	int32 GetCurrentPhase();

	void  SetRotating(bool InValue);
	bool GetIsRotationg();

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
	void SetPatternDecideMode();
	//void SetStrafeMode();


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

	UFUNCTION()
	void OnDecidedPattern_Range(float InValue);

	UFUNCTION()
	void OnDecidedPattern_ID(int32 InID);

	UFUNCTION()
	void OnPhaseChanged(int32 InPhase);

public:
	FAIStateTypeChanged OnAIStateTypeChanged;
	FOnRotated OnRotated;
private:
	class UBlackboardComponent* Blackboard;

private:
	class ACEnemy_AI* CachedAI;

private:
	bool bCanMove = true;
	EAIStateType PrevType;
	int32 Token = -1;
};
