#include "Skill/ActiveSkills/CActiveSkill_Pattern.h"
#include "Global.h"
#include "CActiveSkill_Pattern.h"

void UCActiveSkill_Pattern::DefineSkillPhases()
{
	SetupDefaultSkillPhase();
}

void UCActiveSkill_Pattern::Begin_Skill()
{
	CheckNull(OwnerCharacter);
	CheckFalse(SkillPhaseTable.Contains(ESkillPhase::Begin_Skill));
	CheckFalse(SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas.Num() > 0);

	// 캐스팅 해서 왔으면 동작 수행할 것이고 여기선 그에 대한 처리를 해야한다. 
	SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas[0].Phase_DoAction(OwnerCharacter);
	SkillEntity = SkillPhaseTable[CurrentPhase].PhaseDatas[0].Phase_SpawnSkillEntity(OwnerCharacter);
}

void UCActiveSkill_Pattern::Create_SkillEffect()
{
	CheckFalse(SkillPhaseTable[CurrentPhase].PhaseDatas.Num() > 0);

	SkillPhaseTable[CurrentPhase].PhaseDatas[0].Phase_PlayEffect(OwnerCharacter);
}
