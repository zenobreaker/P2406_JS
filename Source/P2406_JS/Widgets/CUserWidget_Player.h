#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CWeaponComponent.h"
#include "CUserWidget_Player.generated.h"

UCLASS()
class P2406_JS_API UCUserWidget_Player : public UUserWidget
{
	GENERATED_BODY()

public:
	/*UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateWeaponType(EWeaponType InType);*/

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateCrossHairVisibility(bool bInVisible);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateGuardGaugeVisibility(bool bInVisible);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateGuardGauge(float InValue, float InMaxValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateChargeGaugeVisibility(bool bInVisible);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateChargeGauge(float InValue, float InMaxValue);
};
