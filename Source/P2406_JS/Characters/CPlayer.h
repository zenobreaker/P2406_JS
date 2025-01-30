#pragma once

#include "CoreMinimal.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/IStatable.h"
#include "Characters/IDamagable.h"
#include "Characters/IGuardable.h"
#include "Characters/Condition_Interfaces/IAirborne.h"
#include "Characters/Condition_Interfaces/IDownable.h"
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
	, public IIDamagable
	, public IIAirborne
	, public IIDownable
	, public IIGuardable
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "UserInterface")
	TSubclassOf<class UCUserWidget_Player> UiClass;

	UPROPERTY(EditAnywhere, Category = "UserInterface")
	TSubclassOf<class UCUserWidget_SkillHUD> SkillHUDClass;

	UPROPERTY(EditAnywhere, Category = "Evade")
	class UAnimMontage* BackstepMontage;

	UPROPERTY(EditAnywhere, Category = "Jump")
	class UAnimMontage* JumpMontage;


private:
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	FVector2D PitchAngle = FVector2D(-40, +40);

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
	class UCSkillComponent* Skill;

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

	UPROPERTY(VisibleAnywhere)
	class UCAttackTraceComponent* ATrace;

	UPROPERTY(VisibleAnywhere)
	class UCGuardComponent* Guard; 

public:
	FORCEINLINE void SetCanInput(bool bValue) { bCanInput = bValue;  }
	FORCEINLINE bool GetCanInput() { return bCanInput; }

public:
	FORCEINLINE FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(TeamID); }

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
	virtual void Launch(const FHitData& InHitData,const bool bIsGuarding = false);
	virtual void Play_DamageMontage(const struct FHitData& hitData) override;

public:
	void End_Damaged() override;
	void End_Downed() override;

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
	void OnSkill1();
	void OnSkill2();

private:
	bool bEquipped;
	bool bEquipping;


public:
	void Landed(const FHitResult& Hit) override;


private:
	void OnIncreaseTimeScale();
	void OnDecreaseTimeScale();

	void AdjustTimeScale(float InTimeScaleData);

public:
	// IIAirborne을(를) 통해 상속됨
	void OnAirborneConditionActivated() override;
	void OnAirborneConditionDeactivated() override;
	
	void StartDownTimer();
	virtual void OnDownConditionActivated() override;
	virtual void OnDownConditionDeactivated() override;

	// IIGuardable을(를) 통해 상속됨
	bool HasGuard() const override;
	bool CanGuard() const override;
	bool GetGuarding() const override;
	void StartGuard() override;
	void StopGuard() override;


private:
	class UCUserWidget_Player* UserInterface;
	class UCUserWidget_SkillHUD* SkillHUD;

	FTimerHandle ChangeConditionHandle;

private:
	bool bCanInput = true; 
	
public:
	bool* bCountering; 


};
