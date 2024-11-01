#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "CSubAction_Hammer.generated.h"

USTRUCT()
struct FEffectData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* Effect;

	UPROPERTY(EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere)
	FVector EffectScale = FVector::OneVector;

public:
	void PlaySoundWave(class ACharacter* InOwner);
	
	void PlayEffect(class ACharacter* InOwner, const FVector& InLocation);
	void PlayEffect(class ACharacter* InOwner, const FVector& InLocation, const FRotator& InRotator);
};

UCLASS(Blueprintable)
class P2406_JS_API UCSubAction_Hammer : public UCSubAction
{
	GENERATED_BODY()
	
public:
	UCSubAction_Hammer();

public:
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FDoActionData> ActionDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
	class UAnimMontage* ChargeAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
	float MaxChargeTime = 2.0f; 
	
	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FEffectData> Effects;

public:
	void Pressed() override;
	void Released() override; 

	void Tick(float DeltaTime) override;

public:
	void CreateEffect_Charging();
	void CreateEffectSubActionHammer();

private:
	// 충전량 초기화 
	void ResetCharging();
	
private:
	int32 Index; 
	bool IsCharging = false;
	bool bActionable = false; // 충전 액션 가능 여부
	float CurrentChargeTime; 

	FTimerHandle ChargeimeHandle;
	class UNiagaraComponent* NiagaraComponent;
};
