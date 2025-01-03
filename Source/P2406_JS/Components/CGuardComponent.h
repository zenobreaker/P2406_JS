
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

private:
	UFUNCTION()
	void OnHandledTrace(class ACharacter* InAttacker, class AActor* InAttackCauser, class ACharacter* InOther);
	
	UFUNCTION()
	void GuardComp_OnEndTrace();

private:
	void Evaluate_JustTime();

private:
	UPROPERTY(EditAnywhere, Category = "Guard")
	bool bCanGuard = false;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float GuardAngle = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxGuardHP = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	class USoundWave* GuardSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Guard")
	class UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere, Category = "Parry")
	struct FDoActionData ParryActionData;

	UPROPERTY(EditAnywhere, Category = "Parry")
	struct FHitData HitData;

	//class UAnimMontage* ParryMontage;

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
	float GuardHP = 0.0f;

	bool bJustTime = false;

	TArray<class ACharacter*> Hits;
};
