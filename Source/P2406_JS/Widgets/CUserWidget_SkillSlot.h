#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_SkillSlot.generated.h"


UCLASS()
class P2406_JS_API UCUserWidget_SkillSlot : public UUserWidget
{
	GENERATED_BODY()


public:
	virtual void NativeConstruct() override; 

public:
	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void OnSkillSlot(const class UCActiveSkill* InActiveSkill);

	UFUNCTION(BlueprintImplementableEvent, Category = "UserInterface")
	void OnCooldown(int32 InSkillID, float InCooldown);

public:
	void OnSlotClear();

};
