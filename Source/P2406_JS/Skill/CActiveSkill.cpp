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

	// ĳ���� �ð��� �� �Ǿ��ٸ� 
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
		// �ʱ�ȭ �ܰ�
		currentPhase = ESkillPhase::Casting;

		//DoActionDatas[Index].OnAcctionCompleted->AddDyanmic(this, );
		break;
	case ESkillPhase::Begin_Casting:

		
		break;
	case ESkillPhase::Casting:
		// �ִϸ��̼� ����

		break;

	case ESkillPhase::End_Casting:

		break;

	case ESkillPhase::Execution:
		break;

	case ESkillPhase::Effect:
		// ȿ�� ó�� �ܰ�

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
		// ���� �����ͷ� 

		return;
	}

	currentDelay += InTime;
}

/// <summary>
/// ĳ���� ó��
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
/// ��ų ó��
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



