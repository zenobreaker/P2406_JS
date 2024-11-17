#pragma once

#include "CoreMinimal.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/IStatable.h"
#include "Components/CStateComponent.h"
#include "Components/CParkourComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CGrapplingComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CPlayer.generated.h"

UCLASS()
class P2406_JS_API ACPlayer
	: public ACBaseCharacter
	, public IIStatable
	, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 1;

private:
	UPROPERTY(EditAnywhere, Category = "UserInterface")
	TSubclassOf<class UCUserWidget_Player> UiClass;

	UPROPERTY(EditAnywhere, Category = "Evade")
	class UAnimMontage* BackstepMontage;

	UPROPERTY(EditAnywhere, Category = "Jump")
	class UAnimMontage* JumpMontage;


private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* ArrowGroup;

	UPROPERTY(VisibleAnywhere)
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];


private:
	UPROPERTY(VisibleAnywhere)
	class UCHealthPointComponent* HealthPoint;

	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon;

	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* Movement;
	
	UPROPERTY(VisibleAnywhere)
	class UCDashComponent* Dash;

	UPROPERTY(VisibleAnywhere)
	class UCTargetComponent* Target;

	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* State;

	UPROPERTY(VisibleAnywhere)
	class UCParkourComponent* Parkour;

	UPROPERTY(VisibleAnywhere)
	class UCGrapplingComponent* Grapple;

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	UFUNCTION()
	void OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType);

protected:
	void Damaged();

public:
	void End_Damaged() override;

private:
	void OnEvade();
	void Backstep();

public:
	void End_Backstep() override;

private:
	void OnSubAction();
	void OffSubAction();

private:
	void OnJumpAction();
	void OnJumpActionEnd();
	void Jump() override; 

private:
	void OnGrapple();
	void InterruptGrapple();
	
private:
	void OnDash();
	void PlayEvadeEffetc();

private:
	bool bEquipped;
	bool bEquipping;


public:
	void Landed(const FHitResult& Hit) override;

private:
	class UCUserWidget_Player* UserInterface;

};
