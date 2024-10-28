#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "CSubAction_Hammer.generated.h"


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

public:
	void Pressed() override;
	void Released() override; 

	void Tick(float DeltaTime) override;

private:
	void OnPressSpecialAction();

	// 충전량 초기화 
	void StartCharging();
	
private:
	int32 Index; 
	bool IsCharging = false;
	float CurrentChargeTime; 
	
};
