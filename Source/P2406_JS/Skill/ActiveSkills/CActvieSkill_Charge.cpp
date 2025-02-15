#include "Skill/ActiveSkills/CActvieSkill_Charge.h"
#include "Global.h"

void UCActvieSkill_Charge::Tick(float InDeltaTime)
{
	// 기본은 캐스팅 함수 호출하니까 Super 콜 안함
	if (bIsInput == true)
	{
		ChargeTime += InDeltaTime;
		ChargeTime = FMath::Clamp(ChargeTime, 0.0f, MaxChargeTime);
	}
}

ESkillPhase UCActvieSkill_Charge::GetNextFlowPhase(ESkillPhase InPhase)
{
	switch (InPhase)
	{
	case ESkillPhase::Start:
		return  ESkillPhase::Begin_Charging;

	case ESkillPhase::Begin_Charging:
		return  ESkillPhase::End_Charging;

	case ESkillPhase::End_Charging:
		return  ESkillPhase::Begin_Skill;

	case ESkillPhase::Begin_Skill:
		return ESkillPhase::End_Skill;

	case ESkillPhase::End_Skill:
		return ESkillPhase::Finished;
	}

	return ESkillPhase::Max;
}

void UCActvieSkill_Charge::ReleaseSkill()
{
	Super::ReleaseSkill();

	ReleaseCharge();
}


void UCActvieSkill_Charge::StartCharing()
{
	ChargeTime = 0.f;
	bIsInput = true;

	Pressed();
}

void UCActvieSkill_Charge::ReleaseCharge()
{
	bIsInput = false;
}
