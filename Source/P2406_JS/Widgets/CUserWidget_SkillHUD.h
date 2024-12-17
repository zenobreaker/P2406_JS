
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widgets/CUserWidget_Hotbar.h"
#include "CUserWidget_SkillHUD.generated.h"


UCLASS()
class P2406_JS_API UCUserWidget_SkillHUD : public UUserWidget
{
	GENERATED_BODY()
	 
public: 
		
public:
	virtual void NativeConstruct() override;

		// HotBar À§Á¬ º¯¼ö
	UPROPERTY(BlueprintReadOnly, Category = "SkillHUD")
	UCUserWidget_Hotbar* HotBar;
	
public:
	void OnSetOwner(ACharacter* InOwner); 

public:
	UFUNCTION(BlueprintCallable)
	void OnSetSkill(const TArray<class UCActiveSkill*>& InActiveSkills);

	UFUNCTION()
	void OnSetSkillSlotsCleared();

	UFUNCTION()
	void OnUpdateCooldown(int32 InSkillID, float InDeltaTime);
	
private:
	class UCSkillComponent* Skill; 
};
