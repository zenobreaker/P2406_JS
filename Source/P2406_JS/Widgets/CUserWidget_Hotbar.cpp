#include "Widgets/CUserWidget_Hotbar.h"
#include "Global.h"
#include "Widgets/CUserWidget_SkillSlot.h"
#include "Skill/CActiveSkill.h"

void UCUserWidget_Hotbar::OnSetSkillSlots_Implementation(const TArray<UCActiveSkill*>& InActiveSkills)
{
    if (InActiveSkills.Num() != SkillSlots.Num())
        return;

    CLog::Print("SkillSlot- OnSet Slot Call");

    for (int32 i = 0; i < InActiveSkills.Num(); ++i)
    {
        SkillSlots[i]->OnSkillSlot(InActiveSkills[i]);
    }
}


void UCUserWidget_Hotbar::OnSkillCooldown_Implementation(float InCooldown)
{

}
