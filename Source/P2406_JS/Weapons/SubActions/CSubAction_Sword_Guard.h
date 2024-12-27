#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "Components/TimelineComponent.h"
#include "Enums/FEnum.h"
#include "CSubAction_Sword_Guard.generated.h"

UCLASS(Blueprintable)
class P2406_JS_API UCSubAction_Sword_Guard : public UCSubAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxGuardHealth = 10.0f;

public:
	UPROPERTY(EditAnywhere, Category = "Action")
	class UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	class UAnimMontage* GuardHitMontage;
	
	UPROPERTY(EditAnywhere, Category = "Action")
	float GuardAngle = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Action")
	float PlayRate = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Action")
	class USoundWave*  GuardSound; 

	UPROPERTY(EditAnywhere, Category = "Action")
	struct FDoActionData ActionData; 

	UPROPERTY(EditAnywhere, Category = "Action")
	struct FHitData HitData; 

public:
	FORCEINLINE float GetGuardGuage() { return GuardHP; }
	FORCEINLINE float GetMaxGuardGuage() { return MaxGuardHealth; }

public:
	UCSubAction_Sword_Guard();


public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	void Tick(float InDeltaTime) override;

private:
	void OffGuardStance();
public:
	void Pressed() override;
	void Released() override;

public:
	virtual bool TryGuard(struct ACBaseCharacter::FDamageData& DamageData) override;

public:
	UFUNCTION()
	void OnGuard();

	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

public:
	//Notify 
	virtual void Begin_DoSubAction_Implementation() override;
	virtual void End_DoSubAction_Implementation() override;

public:
	//Collision
	UFUNCTION()
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);
	UFUNCTION()
	void OnAttachmentEndCollision();

public:
	void Evaluate_JustTiming(AActor* InAttacker); 
	void Evaluate_GuradStance(const float InValue);
	void Damaged();

private:
	void DebugLine(FVector InAttack, FVector InForward);
	void SetDebug();

private:
	class UCMovementComponent* Movement;
	class UCStateComponent* State; 

private:
	FTimerHandle GuardTimer;
	float GuardHP;


	bool bIsDamaged; 
	bool bIsGuard;
	bool bJustTime; 
	bool bDebug = false;

	TArray<class ACharacter*> Hitted;
};
