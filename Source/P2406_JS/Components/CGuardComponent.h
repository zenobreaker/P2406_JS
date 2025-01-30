#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/CBaseCharacter.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/Guards/CGuardStructures.h"
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


protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool GetCanGuard() const;
	bool GetGuarding() const;
	bool GetCountering() const;

	void SetCanGuard(const bool InValue);

	void OnJustGuard();

public:
	void StartGuard();

	void StopGuard();

	bool CheckBlocking(ACBaseCharacter::FDamageData& InDamageData);

	void CalcGuardHP(const float InDeltaTime = 0.0f);

	void StartCounterGuard();

	void StopCounterGuard();

private:
	void Evaluate_JustTime();

public:
	UFUNCTION()
	void Begin_Parry();

	UFUNCTION()
	void End_Parry();

public:
	void DebugLine(FVector InAttack, FVector InForward);

private:
	UFUNCTION()
	void OnHandledTrace(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOther);

	UFUNCTION()
	void GuardComp_OnEndTrace();

	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType  InNewType);



public:
	FOnGuardDamaged OnGuardDamaged;
	FOnUpdatedGuardVisiable  OnUpdatedGuardVisiable;
	FOnUpdatedGuardGauge OnUpdatedGuardGauge;


private:
	class ACharacter* OwnerCharacter = nullptr;

	class UCWeaponComponent* Weapon = nullptr;
	class UCStateComponent* State = nullptr;
	class UCAttackTraceComponent* ATrace = nullptr;
	class UCMovementComponent* Move = nullptr;


private:
	class UCGuardData* GuardData;
	class UCDoGuard* DoGuard;
	class UCDoParry* DoParry;

private:
	UPROPERTY(EditAnywhere, Category = "Guard")
	class UCGuardDataAsset* GuardAsset;

	bool prevRotationYaw;
};
