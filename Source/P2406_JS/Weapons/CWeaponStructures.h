#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "CWeaponStructures.generated.h"


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
	virtual void DoAction(class ACharacter* InOwner);
	virtual void End_DoAction(class ACharacter* InOwner);
	virtual void Destroy_GhostTrail();
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
	void SendDamage(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, bool bFirstHit = false);
	void PlayHitStop(class ACharacter* InCharacter);
	void PlaySoundWave(class ACharacter* InOwner);
	void PlayEffect(class ACharacter* InCharacter);
	void PlayCameraShake(class ACharacter* InCharacter);
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
