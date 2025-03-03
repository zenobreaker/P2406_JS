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
}

void UCActiveSkill_JudgementBlade::Create_SkillEffect()
{
	// ���� ���� ���� ����� �������� ��ų ����Ʈ ���� 
	
}

void UCActiveSkill_JudgementBlade::Create_Collision()
{
	FLog::Log("Judgement Blade");

}

void UCActiveSkill_JudgementBlade::OnSkillCastingCompleted_JudgementBlade()
{
	// ĳ���� �Ϸ� �Ǹ� ����� �������� �ѱ�
	ExecutePhase(ESkillPhase::End_Casting);
}

void UCActiveSkill_JudgementBlade::OnSkillEnd_JudgementBlade()
{
	// ��ų ���� 
	// ���� ������� ó���ǰ� �ñ�
	CurrentPhase = ESkillPhase::Finished;
}
