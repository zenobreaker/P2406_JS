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
	
	// 여기서 부터 캐스팅을 실시하라고 페이즈 전환
	CurrentCastingTime = 0.0f;

	SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas[0].ExecutePhase(OwnerCharacter);
}

void UCActiveSkill_Instance::Begin_Skill()
{
	CheckNull(OwnerCharacter);
	CheckFalse(SkillPhaseTable.Contains(ESkillPhase::Begin_Skill));
	CheckFalse(SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas.Num() > 0);
	
	// 캐스팅 해서 왔으면 동작 수행할 것이고 여기선 그에 대한 처리를 해야한다. 
	SkillEntity = SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas[0].ExecutePhase(OwnerCharacter);
}
