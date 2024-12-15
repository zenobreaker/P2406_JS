// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Hotbar.generated.h"

/**
 * 
 */
UCLASS()
class P2406_JS_API UCUserWidget_Hotbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TArray<class UCUserWidget_SkillSlot*> SkillSlots;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnSetSkillSlots(const TArray<class UCActiveSkill*>& InActiveSkills); 
	void OnSetSkillSlots_Implementation(const TArray<class UCActiveSkill*>& InActiveSkills);
	

	UFUNCTION(BlueprintNativeEvent)
	void OnSkillCooldown(float InCooldown);
	void OnSkillCooldown_Implementation(float InCooldown);
};
