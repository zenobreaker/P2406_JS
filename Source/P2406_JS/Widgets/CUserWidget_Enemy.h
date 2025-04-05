#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Enemy.generated.h"

UCLASS()
class P2406_JS_API UCUserWidget_Enemy : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  "Hp")
	float HPDecreaseSpeed = 0.0f; 

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateHealth(float InHealth, float InMaxHealth);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateControllerName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateStateName(const FString& InName);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateGuardGaugeVisibility(bool bInVisible);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void UpdateGuardGauge(float InValue, float InMaxValue);

private: 
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
};
