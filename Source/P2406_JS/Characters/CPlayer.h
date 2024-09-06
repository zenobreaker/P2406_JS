#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/IStatable.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "CPlayer.generated.h"

UCLASS()
class P2406_JS_API ACPlayer 
	: public ACharacter
	, public IIStatable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Evade")
	class UAnimMontage* BackstepMontage;
	
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
	void OnEvade();
	void Backstep();

public:
	void End_Backstep() override;

private:
	bool bEquipped;
	bool bEquipping;

};
