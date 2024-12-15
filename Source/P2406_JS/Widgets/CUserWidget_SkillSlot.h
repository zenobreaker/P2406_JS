#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillSlot.generated.h"


UCLASS()
class P2406_JS_API UCUserWidget_SkillSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void OnSkillSlot(class UObject* InActiveSkill);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void OnCooldown(float InCooldown, float InMaxCooldown);
};
