#include "Widgets/CUserWidget_SkillHUD.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Skill/CActiveSkill.h"


void UCUserWidget_SkillHUD::OnSetSkill(const TArray<UCActiveSkill*>& InActiveSkills)
{
	CheckNull(HotBar);
	CLog::Print("HUD - OnSet Skill Call");
	HotBar->OnSetSkillSlots(InActiveSkills);
}
