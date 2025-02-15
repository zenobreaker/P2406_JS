#include "Skill/CSkillAsset.h"
#include "Global.h"
#include "Skill/CActiveSkill.h"

UCSkillAsset::UCSkillAsset()
{

}

void UCSkillAsset::SkillAsset_BeginPlay(ACharacter* InOwner, UCActiveSkill** OutActiveSkill)
{
	CheckNull(SkillClass);

	*OutActiveSkill = NewObject<UCActiveSkill>(GetTransientPackage(), SkillClass);
	if (*OutActiveSkill != nullptr)
	{
		(*OutActiveSkill)->SkillInfo = SkillInfo;
		(*OutActiveSkill)->BeginPlay_ActiveSkill(InOwner, FlowData);
	}
}