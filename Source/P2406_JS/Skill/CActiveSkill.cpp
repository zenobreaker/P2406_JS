#include "Skill/CActiveSkill.h"
#include "Global.h"
#include "Components/CStatecomponent.h"
#include "GameFramework/Character.h"

void UCActiveSkill::BeginPlay(ACharacter* InOwner, const TArray<FSkillActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	DoActionDatas = InDoActionDatas;
	HitDatas = HitDatas;

	CLog::Print("Skill Create!");
}

void UCActiveSkill::Tick(float InDeltaTime)
{
	CasetingSkill(InDeltaTime);
}

void UCActiveSkill::ExecuteSkill()
{
	CheckNull(OwnerCharacter);

	currentPhase = ESkillPhase::Start;

	Index = 0;
	StartNextPhase();
}

void UCActiveSkill::CasetingSkill(float InTime)
{
	if (currentPhase != ESkillPhase::Casting)
		return;

	// 캐스팅 시간이 다 되었다면 
	if (currentCastingTime >= SkillInfo.CastingTime)
	{
		StartNextPhase();
		return;
	}

	currentCastingTime += InTime;

}

void UCActiveSkill::StartNextPhase()
{
	switch (currentPhase)
	{
	case ESkillPhase::Start:
		// 초기화 단계
		currentPhase = ESkillPhase::Casting;

		//DoActionDatas[Index].OnAcctionCompleted->AddDyanmic(this, );
		break;
	case ESkillPhase::Begin_Casting:

		
		break;
	case ESkillPhase::Casting:
		// 애니메이션 시작

		break;

	case ESkillPhase::End_Casting:

		break;

	case ESkillPhase::Execution:
		break;

	case ESkillPhase::Effect:
		// 효과 처리 단계

		currentPhase = ESkillPhase::Finished;
		break;
	}
}

void UCActiveSkill::StartPhase(ESkillPhase phase)
{
	currentPhase = phase; 

	StartNextPhase();
}

void UCActiveSkill::DelayNextData(float InTime)
{
	if (currentDelay >= DoActionDatas[Index].HitDelay)
	{
		// 다음 데이터로 

		return;
	}

	currentDelay += InTime;
}

/// <summary>
/// 캐스팅 처리
/// </summary>
void UCActiveSkill::DoActionData()
{
	if (Index >= DoActionDatas.Num())
		return;

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCActiveSkill::Begin_Casting()
{
	DoActionDatas[Index].Begin_Casting(OwnerCharacter);
}

void UCActiveSkill::DoCasting()
{
	DoActionDatas[Index].DoCasting(OwnerCharacter);
}

void UCActiveSkill::End_Casting()
{
	DoActionDatas[Index].End_Casting(OwnerCharacter);
}


/// <summary>
/// 스킬 처리
/// </summary>

void UCActiveSkill::Begin_Skill()
{

}

void UCActiveSkill::End_Skill()
{

}

void UCActiveSkill::Create_Effect()
{

}



