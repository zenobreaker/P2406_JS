#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "../IComboable.h"
#include "CDoAction_JumpAction.generated.h"

UENUM()
enum class FallAttackState
{
	Begin, Loop, End, Max
};

USTRUCT()
struct FallingAttackEffect 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* DownEffect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::OneVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;
};

UCLASS(Blueprintable)
class P2406_JS_API UCDoAction_JumpAction
	: public UCDoAction
	, public IIComboable
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere, Category = "Effect")
	struct FallingAttackEffect FallingEffect;

public:
	FORCEINLINE void EnableCombo() override { bEnable = true; }
	FORCEINLINE void DisableCombo() override { bEnable = false; }

public:
	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	);

	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas,
		const TArray<FDoActionData>& InFallActionDatas,
		const TArray<FHitData>& InFallHitDatas
	);

	void Tick(float InDelaTime) override;

	void InitializeFallAttackData(
		const TArray<FDoActionData>& InFallActionDatas,
		const TArray<FHitData>& InHitDatas);


public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;
	
public:
	void SetHeavyActionFlag(bool InValue) override;

public:
	void OnAttachmentEndCollision() override;

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker,class AActor* InAttackCauser, class AActor* InOther) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InOther) override;

public:
	void DoAction_FallAttackFlow(FallAttackState InState);
	void Begin_DoAction_FallAttack();
	void End_DoAction_FallAttack();
	void Play_DownEffect();

public:
	void PlayFallAttackMontage() override;

private:
	void ChecFallAttackHit();

private: 
	void SetFlyMode();

	UFUNCTION()
	void SetFallMode(); 

	UFUNCTION()
	void Lanaded_FallAttack();

	UFUNCTION()
	void OnLanded();

private:
	int32 Index;

	bool bEnable;
	bool bExist;


private:
	FallAttackState FAState = FallAttackState::Max;

private:
	float Gravity;
	float Velocity;

	bool bIsFallAttack = false; 

private:
	class UCWeaponComponent* Weapon;

private:
	TArray<FDoActionData> FallActionDatas;
	TArray<FHitData> FallHitDatas;
	class ACSkillCollision* FallActionCollison;


	ECollisionResponse PreviousCollisionResponse;

	TArray<class AActor*> Hitted;
};
