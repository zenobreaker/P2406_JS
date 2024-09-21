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

};
