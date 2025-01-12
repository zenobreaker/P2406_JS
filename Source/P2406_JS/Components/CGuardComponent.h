
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/CBaseCharacter.h"
#include "Weapons/CWeaponStructures.h"
#include "CGuardComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGuardDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedGuardVisiable, bool, bInVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatedGuardGauge, float, InValue, float, InMaxValue);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCGuardComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCGuardComponent();

public:
	FORCEINLINE bool GetCanGuard() const { return bCanGuard; }
	FORCEINLINE bool GetGuarding() const { return bGuarding; }

	FORCEINLINE void SetCanGuard(const bool InValue) { bCanGuard = InValue; }


protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnJustGuard();

public:
	void StartGuard();

	void StopGuard();

	bool CheckBlocking(ACBaseCharacter::FDamageData& InDamageData);

	void CalcGuardHP(const float InDeltaTime = 0.0f);

	void CalcGuardCooldown(const float InDeltaTime);

	void StartCounterGuard();

	void StopCounterGuard();

	void DebugLine(FVector InAttack, FVector InForward);

private:
	UFUNCTION()
	void OnHandledTrace(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
	void GuardComp_OnEndTrace();

	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType  InNewType);

private:
	void Evaluate_JustTime();

private:
	UPROPERTY(EditAnywhere, Category = "Guard")
	bool bCanGuard = true;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float GuardAngle = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxGuardHP = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxGuardCooldown = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	class USoundWave* GuardSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Guard")
	class UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere, Category = "Parry")
	struct FDoActionData ParryActionData;

	UPROPERTY(EditAnywhere, Category = "Parry")
	struct FHitData HitData;

	//class UAnimMontage* ParryMontage;
private:
	UPROPERTY(EditAnywhere, Category = "Counter")
	float MaxCounterTime = 5.0f; 

	UPROPERTY(EditAnywhere, Category = "Counter")
	float MaxCounterRecoveryTime = 7.0f; 


	UPROPERTY(EditAnywhere, Category = "Counter")
	class UAnimMontage* CounterWaitMontage; 

public:
	FOnGuardDamaged OnGuardDamaged;
	FOnUpdatedGuardVisiable  OnUpdatedGuardVisiable;
	FOnUpdatedGuardGauge OnUpdatedGuardGauge;


private:
	class ACharacter* OwnerCharacter = nullptr;
	class UCStateComponent* State = nullptr;
	class UCAttackTraceComponent* ATrace = nullptr;

private:
	bool bGuarding = false;
	bool bCountering = false;

	float GuardHP = 0.0f;
	float GuardCooldown = 0.0f; 
	float CounterTime = 0.0f; 
	float CounterRecoveryTime = 0.0f;

	bool bJustTime = false;

	TArray<class ACharacter*> Hits;
};
