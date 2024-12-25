#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Enemy.generated.h"

UCLASS()
class P2406_JS_API UCUserWidget_Enemy : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateControllerName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateStateName(const FString& InName);
};
