#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CGuardStructures.h"
#include "CGuardDataAsset.generated.h"


UCLASS()
class P2406_JS_API UCGuardDataAsset : public UDataAsset
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, Category = "Guard")
	TSubclassOf<class UCDoGuard> DoGuardClass;

	UPROPERTY(EditAnywhere, Category = "Guard")
	FGuardData GuardData; 

	UPROPERTY(EditAnywhere, Category = "Guard Hit")
	FGuardHitData GuardHitData; 

	UPROPERTY(EditAnywhere, Category = "Counter")
	FCounterData CounterData;

	UPROPERTY(EditAnywhere, Category = "Parry")
	TSubclassOf<class UCDoParry> DoParryClass;

	UPROPERTY(EditAnywhere, Category = "Parry")
	FParryData ParryData;

public:
	UCGuardDataAsset();

	void BeginPlay(class ACharacter* InOwnerCharacter, class UCGuardData** OutGuardData);
};


/*UPROPERTY(EditAnywhere, Category = "Guard")
	float GuardAngle = 60.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxGuardHP = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxGuardCooldown = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Guard")
	class USoundWave* GuardSound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Guard")
	class UAnimMontage* GuardMontage;*/
	


	//UPROPERTY(EditAnywhere, Category = "Parry")
	//struct FDoActionData ParryActionData;

	//UPROPERTY(EditAnywhere, Category = "Parry")
	//struct FHitData HitData;

	////class UAnimMontage* ParryMontage;

	//UPROPERTY(EditAnywhere, Category = "Counter")
	//float MaxCounterTime = 5.0f; 

	//UPROPERTY(EditAnywhere, Category = "Counter")
	//float MaxCounterRecoveryTime = 7.0f; 


	//UPROPERTY(EditAnywhere, Category = "Counter")
	//class UAnimMontage* CounterWaitMontage; 