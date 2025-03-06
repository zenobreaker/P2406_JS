#include "Skill/ActiveSkills/CActiveSkill_Instance.h"
#include "Global.h"

void UCActiveSkill_Instance::DefineSkillPhases()
{
	Super::DefineSkillPhases();
	
	SetupDefaultSkillPhase();
}

void UCActiveSkill_Instance::Begin_Casting()
{
	CheckNull(OwnerCharacter);
	CheckFalse(SkillPhaseTable.Contains(ESkillPhase::Begin_Casting));
	CheckFalse(SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas.Num() > 0);
	
	// ���⼭ ���� ĳ������ �ǽ��϶�� ������ ��ȯ
	CurrentCastingTime = 0.0f;

	SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas[0].ExecutePhase(OwnerCharacter);
}

void UCActiveSkill_Instance::Begin_Skill()
{
	CheckNull(OwnerCharacter);
	CheckFalse(SkillPhaseTable.Contains(ESkillPhase::Begin_Skill));
	CheckFalse(SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas.Num() > 0);
	
	// ĳ���� �ؼ� ������ ���� ������ ���̰� ���⼱ �׿� ���� ó���� �ؾ��Ѵ�. 
	SkillEntity = SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas[0].ExecutePhase(OwnerCharacter);
}
