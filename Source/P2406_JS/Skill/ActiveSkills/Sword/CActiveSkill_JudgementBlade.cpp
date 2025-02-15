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
	
	// 여기서 부터 캐스팅을 실시하라고 페이즈 전환
	currentCastingTime = 0.0f; 

	if (SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas.Num() > 0)
		SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas[0].ExecutePhase(OwnerCharacter);

	REGISTER_EVENT_WITH_REPLACE(this, OnSkillCastingCompleted, this, UCActiveSkill_JudgementBlade::OnSkillCastingCompleted_JudgementBlade);

}

void UCActiveSkill_JudgementBlade::Begin_Skill()
{
	CheckNull(OwnerCharacter);
	// 캐스팅 해서 왔으면 동작 수행할 것이고 여기선 그에 대한 처리를 해야한다. 
	if (SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas.Num() > 0)
		SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas[0].ExecutePhase(OwnerCharacter);
	
	
	//TODO: 없어도 될듯 해당 페이즈 없으면 자동으로 넘기니까 
	//REGISTER_EVENT_WITH_REPLACE(this, OnSkillEnded, this, UCActiveSkill_JudgementBlade::OnSkillEnd_JudgementBlade);
}

void UCActiveSkill_JudgementBlade::Create_SkillEffect()
{
	// 현재 진행 중인 페이즈를 기준으로 스킬 이펙트 생성 
	
}

void UCActiveSkill_JudgementBlade::Create_Collision()
{
	FLog::Log("Judgement Blade");

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
