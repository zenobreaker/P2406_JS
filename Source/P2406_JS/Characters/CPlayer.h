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


UENUM()
enum class EInputState
{
	Combat,
	Grapple,
	Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputStateChange, EInputState, InState);

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

	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpValue = -1.0f;

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

	UPROPERTY(VisibleAnywhere)
	class UCStatComponent* Stat;

	UPROPERTY(VisibleAnywhere)
	class UCBuffComponent* Buff;

	UPROPERTY(VisibleAnywhere)
	class UCZoomComponent* ZoomC;

public:
	FORCEINLINE void SetCanInput(bool bValue) { bCanInput = bValue; }
	FORCEINLINE bool GetCanInput() { return bCanInput; }

public:
	FORCEINLINE FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(TeamID); }

public:
	ACPlayer();

private:
	void CreateActorComponent();
	void CreateArrowGroup();

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

	UFUNCTION()
	virtual void OnHealthPointChanged(float InHealth, float InMaxHealth);

public:
	void OnInputStateChanged(EInputState InState);

protected:
	void Damaged();
	virtual void Launch(const FHitData& InHitData, const bool bIsGuarding = false);
	virtual void Play_DamageMontage(const struct FHitData& hitData) override;

public:
	void End_Damaged() override;
	void End_Downed() override;

	void OnVisibilityUI();
	void OnHiddenUI();

public:
	void VisibleBossGauge();

	UFUNCTION()
	void SetUpBossUI(class ACBoss_AI* Boss);

	UFUNCTION()
	void HideBossGauge();

	UFUNCTION()
	void UpdateBossGauge(float InValue, float MaxValue);

private:
	void OnEvade();
	void Backstep();

public:
	void End_Backstep() override;


private:
	void OnAction();

private:
	void OnSubAction();
	void OffSubAction();

	void OnExecute();
	void OffExecute();

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
	void ReleaseSkill1();
	void OnSkill2();
	void ReleaseSkill2();
	void OnSkill3();
	void ReleaseSkill3();
	void OnSkill4();
	void ReleaseSkill4();

public:
	void Landed(const FHitResult& Hit) override;

private:
	void FindComponent();

private:
	void OnIncreaseTimeScale();
	void OnDecreaseTimeScale();

	void AdjustTimeScale(float InTimeScaleData);

	void OnSwitchInfoHUD();

public:
	// IIAirborne을(를) 통해 상속됨
	void OnAirborneConditionActivated() override;
	void OnAirborneConditionDeactivated() override;

	void StartDownTimer();
	void OnDownConditionActivated() override;
	void OnDownConditionDeactivated() override;

	// IIGuardable을(를) 통해 상속됨
	bool HasGuard() const override;
	bool CanGuard() const override;
	bool GetGuarding() const override;
	void StartGuard() override;
	void StopGuard() override;

	void Test_Invicible();

	void OnSpectoringCamear();


public:
	FOnInputStateChange OnInputStateChange;

private:
	class APawn* SpectorCamPawn;
	class APawn* OriginalPawn;

private:
	class UCUserWidget_Player* UserInterface;
	class UCUserWidget_SkillHUD* SkillHUD;

	FTimerHandle ChangeConditionHandle;

private:
	EInputState inputState;

private:
	bool bEquipped;
	bool bEquipping;


private:
	bool bCanInput = true;
	bool bOnHUD = false;

public:
	bool* bCountering;


};
