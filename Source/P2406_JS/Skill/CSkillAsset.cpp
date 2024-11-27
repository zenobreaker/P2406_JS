#include "Skill/CSkillAsset.h"
#include "Global.h"
#include "Skill/CActiveSkill.h"

UCSkillAsset::UCSkillAsset()
{

}

void UCSkillAsset::BeginPlay(ACharacter* InOwner, UCActiveSkill** OutActiveSkill)
{
	*OutActiveSkill = NewObject<UCActiveSkill>();
	(*OutActiveSkill)->SkillInfo = SkillInfo;
	(*OutActiveSkill)->BeginPlay(InOwner, DoActionDatas, HitDatas);
}