#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class P2406_JS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Sword")
	class UAnimMontage* EquipMontage;
	
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

private:
	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent*Movement;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

private:
	void Equip();

private:
	class ACSword* Sword;

private:
	bool bEquipped;
	bool bEquipping;

};
