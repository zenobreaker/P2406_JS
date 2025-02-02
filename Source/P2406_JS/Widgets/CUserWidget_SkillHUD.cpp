#include "Widgets/CUserWidget_SkillHUD.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Skill/CActiveSkill.h"
	
void UCUserWidget_SkillHUD::NativeConstruct()
{
	Super::NativeConstruct(); 

	//FLog::Print("SkillHUD - NativeConstruct called");

    HotBar = Cast<UCUserWidget_Hotbar>(GetWidgetFromName(TEXT("WB_CSkillHotBar")));
 /*   if (!!HotBar)
    {
        FLog::Print("SkillHUD - Success to initialize HotBar", -1, 10.0, FColor::Green);
    }
    else
    {
        FLog::Print("SkillHUD - Failed to initialize HotBar", -1, 10.0, FColor::Red);
    }*/
}


void UCUserWidget_SkillHUD::OnSetSkill(const TArray<UCActiveSkill*>& InActiveSkills)
{
    CheckNull(HotBar);

	//FLog::Print("HUD - OnSet Skill Call");
	HotBar->OnSetSkillSlots(InActiveSkills);
}

void UCUserWidget_SkillHUD::OnSetSkillSlotsCleared()
{
    CheckNull(HotBar);
    HotBar->OnSetSkillSlotsCleared();
}

void UCUserWidget_SkillHUD::OnUpdateCooldown(int32 InSkillID, float InDeltaTime)
{
    CheckNull(HotBar);
    HotBar->OnUpdateCooldown(InSkillID, InDeltaTime);
}
