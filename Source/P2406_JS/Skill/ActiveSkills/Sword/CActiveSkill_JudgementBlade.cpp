#include "Skill/ActiveSkills/Sword/CActiveSkill_JudgementBlade.h"
#include "Global.h"

#include "Skill/CActiveSkill.h"


void UCActiveSkill_JudgementBlade::DefineSkillPhases()
{
	Super::DefineSkillPhases();

	//AssignSkillPhase(TDelegate<void()>::CreateUObject(this, &UCActiveSkill::Begin_Casting));
	//AssignSkillPhase(TDelegate<void()>::CreateUObject(this, &UCActiveSkill::Begin_Skill));

	//AssignSkillPhase(ESkillPhase::Begin_Casting, UCActiveSkill_JudgementBlade::Begin_Casting);
}

void UCActiveSkill_JudgementBlade::Begin_Casting()
{
	CheckNull(OwnerCharacter); 
	
	// ���⼭ ���� ĳ������ �ǽ��϶�� ������ ��ȯ
	currentCastingTime = 0.0f; 

	if (SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas.Num() > 0)
		SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas[0].ExecutePhase(OwnerCharacter);

	REGISTER_EVENT_WITH_REPLACE(this, OnSkillCastingCompleted, this, UCActiveSkill_JudgementBlade::OnSkillCastingCompleted_JudgementBlade);

}

void UCActiveSkill_JudgementBlade::Begin_Skill()
{
	CheckNull(OwnerCharacter);
	// ĳ���� �ؼ� ������ ���� ������ ���̰� ���⼱ �׿� ���� ó���� �ؾ��Ѵ�. 
	if (SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas.Num() > 0)
		SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas[0].ExecutePhase(OwnerCharacter);
	
	
	//TODO: ��� �ɵ� �ش� ������ ������ �ڵ����� �ѱ�ϱ� 
	//REGISTER_EVENT_WITH_REPLACE(this, OnSkillEnded, this, UCActiveSkill_JudgementBlade::OnSkillEnd_JudgementBlade);
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
