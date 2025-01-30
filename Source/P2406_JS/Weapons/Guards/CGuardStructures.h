#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../CWeaponStructures.h"
#include "CGuardStructures.generated.h"



USTRUCT()
struct FGuardData
{
	GENERATED_BODY()

public:

	//bool bCanGuard;
	UPROPERTY(EditAnywhere)
	UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float GuardAngle = 60.0f;

	UPROPERTY(EditAnywhere)
	float MaxGuardHP = 10.0f;

	UPROPERTY(EditAnywhere)
	float MaxGuardCooldown = 10.0f;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true; 

	UPROPERTY(EditAnywhere)
	USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class UCameraShakeBase> CameraShake; 


public:
	void DoGuard(class ACharacter* InOwnerCharacter) const;
	void StopGuard(class ACharacter* InOwnerCharacter) const;

	void PlaySoundWave(class ACharacter* InOwnerCharacter);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);
};


USTRUCT()
struct FGuardHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* GuardHitMontage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

	//UPROPERTY(EditAnywhere)
	//TSubclassOf<class UCameraShakeBase> CameraShake; 


public:
	void DoGuardHit(class ACharacter* InOwnerCharacter);

	void PlaySoundWave(class ACharacter* InOwnerCharacter);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);
};

USTRUCT()
struct FCounterData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	UAnimMontage* CounterWaitMontage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float MaxCounterTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxCounterRecoveryTime = 7.0f;

	UPROPERTY(EditAnywhere)
	USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UFXSystemAsset* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;


public:
	void DoAction_CounterWait(class ACharacter* InOwnerCharacter);
	void End_CounterWait(class ACharacter* InOwnerCharacter);

	void PlaySoundWave(class ACharacter* InOwnerCharacter);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);
};

USTRUCT()
struct FParryData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FDoActionData ParryActionData;

	UPROPERTY(EditAnywhere)
	FHitData HitData;

public:
	void DoAction_Parry(class ACharacter* InOwnerCharacter);
};


UCLASS()
class P2406_JS_API UCGuardStructures : public UObject
{
	GENERATED_BODY()
};
