#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Hotbar.generated.h"


UCLASS()
class P2406_JS_API UCUserWidget_Hotbar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	int32 MaxSlots = 4;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class UCUserWidget_SkillSlot*> SkillSlots;

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnSetSkillSlots(const TArray<class UCActiveSkill*>& InActiveSkills); 
	void OnSetSkillSlots_Implementation(const TArray<class UCActiveSkill*>& InActiveSkills);
	
	UFUNCTION()
	void OnSetSkillSlotsCleared();


	UFUNCTION(BlueprintNativeEvent)
	void OnUpdateCooldown(int32 InSkillID, float InDeltaTime);
	void OnUpdateCooldown_Implementation(int32 InSkillID, float InDeltaTime);


};
