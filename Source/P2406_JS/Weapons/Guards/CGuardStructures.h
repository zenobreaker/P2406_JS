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
	class UAnimMontage* GuardMontage = nullptr;

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> CameraShake; 


public:
	void Begin_Guard(class ACharacter* InOwnerCharacter = nullptr) const;
	void End_Guard(class ACharacter* InOwnerCharacter = nullptr) const;

	void PlaySoundWave(class ACharacter* InOwnerCharacter = nullptr);
	void PlayCameraShake(class ACharacter* InCharacter);
	void PlayEffet(class ACharacter* InCharacter);

	/*void PlayEffect(UWorld* InWorld, const FVector& InLocation);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);*/
};


USTRUCT()
struct FGuardHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* GuardHitMontage = nullptr;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;

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
	class UAnimMontage* CounterWaitMontage = nullptr;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	float MaxCounterTime = 5.0f;

	UPROPERTY(EditAnywhere)
	float MaxCounterRecoveryTime = 7.0f;

	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;

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
