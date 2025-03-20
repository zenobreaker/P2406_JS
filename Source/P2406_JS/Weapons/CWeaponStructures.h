#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CWeaponStructures.generated.h"

USTRUCT()
struct FAttachmentTrailData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName StartSocket = NAME_None;

	UPROPERTY(EditAnywhere)
	FName EndSocket = NAME_None;

	UPROPERTY(EditAnywhere)
	TEnumAsByte<enum ETrailWidthMode> WidthScaleMode;

	UPROPERTY(EditAnywhere)
	float Width = 1.0f;

public:
	UPROPERTY(EditAnywhere)
	class UParticleSystem* TrailEffet;

public:
	UPROPERTY(EditAnywhere)
	FName DustSocket = NAME_None;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* DustEffect;
};

USTRUCT()
struct FAttachmentSocketData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName StartSocket = NAME_None;

	UPROPERTY(EditAnywhere)
	FName EndSocket = NAME_None;
};

USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bUseControlRotation = true;
};


USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera = false;
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class ACGhostTrail> GhostTrailClass;*/

//private:
//	class ACGhostTrail* GhostTrail;

public:
	virtual void DoAction(class ACharacter* InOwner, /*bool IsImmedate = false, */FName InSectionName = NAME_None);
	virtual void End_DoAction(class ACharacter* InOwner);
	virtual void Destroy_GhostTrail();
	virtual void AnimationPlayback(class ACharacter* InOwner, float InValue);
};

USTRUCT()
struct FHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	float Power =0.0f;

	UPROPERTY(EditAnywhere)
	float Launch = 100;

	UPROPERTY(EditAnywhere)
	float Airial = 0.0f;

	UPROPERTY(EditAnywhere)
	float StopTime =0.0f;

	UPROPERTY(EditAnywhere)
	bool bDown = false; 

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect; 

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake; 


public:
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class AActor* InOther, bool bFirstHit = false);
	void PlayHitStop(class AActor* InActor);
	void PlaySoundWave(class AActor* InActor);
	void PlayEffect(class AActor* InActor);
	void PlayCameraShake(class AActor* InActor);
};

USTRUCT()
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	bool bFirstHit;

	FHitData* HitData;
};


UCLASS()
class P2406_JS_API UCWeaponStructures : public UObject
{
	GENERATED_BODY()
};
