
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UCUserWidget_Hotbar* HotBar;

public:
	UFUNCTION()
	void OnSetSkill(const TArray<class UCActiveSkill*>& InActiveSkills);

};
