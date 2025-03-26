#include "Skill/ActiveSkills/Sword/CActiveSkill_JudgementBlade.h"
#include "Global.h"

#include "Skill/CActiveSkill.h"


void UCActiveSkill_JudgementBlade::DefineSkillPhases()
{
	Super::DefineSkillPhases();
}

void UCActiveSkill_JudgementBlade::Begin_Casting()
{
	Super::Begin_Casting();

	REGISTER_EVENT_WITH_REPLACE(this, OnSkillCastingCompleted, this, UCActiveSkill_JudgementBlade::OnSkillCastingCompleted_JudgementBlade);
}

void UCActiveSkill_JudgementBlade::Begin_Skill()
{
	Super::Begin_Skill();

	//OnActivated_Collision();
}

void UCActiveSkill_JudgementBlade::OnSkillCastingCompleted_JudgementBlade()
{
	// 캐스팅 완료 되면 페이즈를 다음으로 넘김
	ExecutePhase(ESkillPhase::End_Casting);
}

void UCActiveSkill_JudgementBlade::OnSkillEnd_JudgementBlade()
{
	// 스킬 종료 
	// 다음 페이즈에서 처리되게 맡김
	CurrentPhase = ESkillPhase::Finished;
}
