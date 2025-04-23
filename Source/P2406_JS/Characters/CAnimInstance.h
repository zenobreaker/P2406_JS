#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Components/CWeaponComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CSkillComponent.h"
#include "Components/CFeetComponent.h"
#include "CAnimInstance.generated.h"


UCLASS()
class P2406_JS_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bFalling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bFallingAttack;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bGrappling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bIsAirborneHit;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bDown;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float DownBlendValue;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	bool bUseFootIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FFeetData FeetData;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	float LeftIKValue = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	float RightIKValue = 1.0f;


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bBow_Aiming;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	bool bGuarding = false;

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EStateType StateType = EStateType::Max;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Owner")
	bool bPlayer = true;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;


private:
	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnCharacterLanded();

	UFUNCTION()
	void OnCharacterDowned();

	UFUNCTION()
	void OnCharacterRaised();

private:
	void ChangeFalling();

	void ChangeBowAiming();

	void ChangeGrappling();

	void ChangeAirborne();

	void ChangeDown();

	void ChangeGuardState();

	void ChangeFeet();

	float GetIKValue(float InValue1, float InValue2);

	void ChangeFallingAttack();
private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent* Weapon;
	class UCStateComponent* State;
	class UCSkillComponent* Skill;
	class UCGrapplingComponent* Grapple;
	class UCConditionComponent* Condition;
	class UCFeetComponent* Feet;
	class UCParkourComponent* Parkour;

private:
	FRotator PrevRotation;
};
