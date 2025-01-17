#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/FEnum.h"
#include "CAIBehaviorComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChanged, EAIStateType, InPrevType, EAIStateType, InNewType);



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()


public:
	FORCEINLINE bool GetCanMove() const { return bCanMove; }

	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }

private:
	UPROPERTY(EditAnywhere, Category = "Key")
	FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatrolLocationKey = "PatrolLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName EqsLocationKey = "EqsLocation";

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

	FVector GetPatrolLocation();
	void SetPatrolLocation(const FVector& InLocation);

	FVector GetEqsLocation();
	void SetEqsLocation(const FVector& InLocation);

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetDamageMode();
	void SetAvoidMode();
	void SetAriborneMode();
	void SetDownMode();
	void SetGuardMode();
	void SetDeadMode();

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

public:
	FAIStateTypeChanged OnAIStateTypeChanged;



private:
	class UBlackboardComponent* Blackboard;

private:
	class ACEnemy_AI* CachedAI;

	bool bCanMove = true;

};
