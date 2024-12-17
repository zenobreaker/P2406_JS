#include "Widgets/CUserWidget_SkillHUD.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Skill/CActiveSkill.h"
#include "Components/CSkillComponent.h"

	
void UCUserWidget_SkillHUD::NativeConstruct()
{
	Super::NativeConstruct(); 

	CLog::Print("SkillHUD - NativeConstruct called");

    HotBar = Cast<UCUserWidget_Hotbar>(GetWidgetFromName(TEXT("WB_CSkillHotBar")));
    if (!!HotBar)
    {
        CLog::Print("SkillHUD - Success to initialize HotBar", -1, 10.0, FColor::Green);
    }
    else
    {
        CLog::Print("SkillHUD - Failed to initialize HotBar", -1, 10.0, FColor::Red);
    }
}


void UCUserWidget_SkillHUD::OnSetSkill(const TArray<UCActiveSkill*>& InActiveSkills)
{
    CheckNull(HotBar);

	CLog::Print("HUD - OnSet Skill Call");
	HotBar->OnSetSkillSlots(InActiveSkills);
}

void UCUserWidget_SkillHUD::OnSetOwner(ACharacter* InOwner)
{
    CheckNull(InOwner); 

    Skill = CHelpers::GetComponent<UCSkillComponent>(InOwner); 
    CheckNull(Skill); 

    CLog::Print("Skill HUD Delegate Set", -1, 10, FColor::Magenta);
    Skill->OnSetSkills.AddDynamic(this, &UCUserWidget_SkillHUD::OnSetSkill); 

    Skill->OnSkillSlotsCleared.AddDynamic(this, &UCUserWidget_SkillHUD::OnSetSkillSlotsCleared);

    //TODO: 정말 개념이 없는 행위지만 일단은..
    
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
