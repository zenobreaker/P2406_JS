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

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "BossGauge")
	void UpdateBossGaugeVisibility(bool bInVisible); 

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "BossGauge")
	void UpdateBossGauge(float InValue, float InMaxValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateHealth(float InValue, float InMaxValue);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void VisibleInfo(bool InToggle, class ACharacter* InOwner);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void ShowInputPrompt(FKey InKey);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void HideInputPrompt();
};
