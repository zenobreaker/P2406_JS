#include "Widgets/CUserWidget_Hotbar.h"
#include "Global.h"
#include "Widgets/CUserWidget_SkillSlot.h"
#include "Skill/CActiveSkill.h"

void UCUserWidget_Hotbar::NativeConstruct()
{
	Super::NativeConstruct();

	FLog::Print("HotBar - NativeConstruct called");
	// Slots √ ±‚»≠
}

void UCUserWidget_Hotbar::OnSetSkillSlots_Implementation(const TArray<UCActiveSkill*>& InActiveSkills)
{
	CheckTrue(InActiveSkills.Num() <= 0);
	check(SkillSlots.Num() > 0);

	FLog::Print("Hot bar  : Call Set Skill Slot ", -1, 10.0f, FColor::Yellow);
	int cnt = 0;
	for (const UCActiveSkill* activeSkill : InActiveSkills)
	{
		if (activeSkill == nullptr)
			continue;

		SkillSlots[cnt]->OnSkillSlot(activeSkill);
		cnt++;
	}
}

void UCUserWidget_Hotbar::OnSetSkillSlotsCleared()
{
	for (auto& slot : SkillSlots)
	{
		slot->OnSlotClear();
	}
}


void UCUserWidget_Hotbar::OnUpdateCooldown_Implementation(int32 InSkillID, float InDeltaTime)
{
	for (auto& slot : SkillSlots)
	{
		slot->OnCooldown(InSkillID, InDeltaTime);
	}
}


//void TEST()
//{
//    //if (InActiveSkills.Num() != SkillSlots.Num())
//    //{
//    //    FLog::Print("SkillSlot- OnSet Slot Failed " + FString::FromInt(MaxSlots) + " " + FString::FromInt(InActiveSkills.Num()), -1, 10.0f, FColor::Red);
//    //    return;
//    //}
//
//    //FLog::Print("SkillSlot- OnSet Slot Call", -1, 10.0f, FColor::Black);
//
//    //for (int32 i = 0; i < InActiveSkills.Num(); ++i)
//    //{
//    //    SkillSlots[i]->OnSkillSlot(InActiveSkills[i]);
//    //}
//}