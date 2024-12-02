#include "Skill/CActiveSkill.h"
#include "Global.h"
#include "Components/CStatecomponent.h"
#include "GameFramework/Character.h"

void UCActiveSkill::BeginPlay(ACharacter* InOwner, const TArray<FSkillActionData>& InDoActionDatas, const TArray<FSkillHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	DoActionDatas = InDoActionDatas;
	HitDatas = HitDatas;

	//OnActionCompleted.AddDynamic(this, &UCActiveSkill::StartNextPhase);

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

void UCActiveSkill::EndSkill()
{
	if (currentPhase == ESkillPhase::DoAction_Skill)
		StartNextPhase();
}

void UCActiveSkill::CasetingSkill(float InTime)
{
	if (currentPhase != ESkillPhase::Casting)
		return;

	if (currentCastingTime >= SkillInfo.CastingTime)
	{
		StartNextPhase();
		return; 
	}

	currentCastingTime += InTime;

}

void UCActiveSkill::StartNextPhase()
{
	if (currentPhase == ESkillPhase::Max)
	{
		CLog::Print("Skill Phase All Complete");
		return; 
	}

	// �ٸ� ������� ��ȯ
	currentPhase = StaticCast<ESkillPhase>(StaticCast<int8>(currentPhase) + 1);
	ExecutePhase(currentPhase);
}

void UCActiveSkill::ExecutePhase(ESkillPhase phase)
{
	switch (phase)
	{
	case ESkillPhase::Begin_Casting:

		Begin_Casting();

		break;
	case ESkillPhase::Casting:
		// �ִϸ��̼� ����
		DoCasting();

		break;
	case ESkillPhase::End_Casting:
		End_Casting();

		break;

	case ESkillPhase::Begin_Skill:
		Begin_Skill();

		break;
	case ESkillPhase::DoAction_Skill:

		DoAction_Skill();

		break;
	case ESkillPhase::End_Skill:
		End_Skill();

		break;
	case ESkillPhase::Effect:
		// ȿ�� ó�� �ܰ�
		break;
	}
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
void UCActiveSkill::Begin_Casting()
{
	CLog::Print("Skill : Begin_Casting");
	// �������� �Լ� ���� 
	DoActionDatas[Index].Begin_Casting(OwnerCharacter);
	
	//���� ������ : DoCasting�� �ű� 
	StartNextPhase();
}

void UCActiveSkill::DoCasting()
{
	CLog::Print("Skill : DoCasting");
	DoActionDatas[Index].DoCasting(OwnerCharacter);
	// ���� ������ ó���� Tick���� �� 
}

void UCActiveSkill::End_Casting()
{
	CLog::Print("Skill : End_Casting");
	DoActionDatas[Index].End_Casting(OwnerCharacter);
	
	// ĳ������ �������Ƿ� �����Ϳ� ó���Ұ� ������ ó���϶�� �ϰ� ���� ������� ����
	StartNextPhase(); 
}


/// <summary>
/// ��ų ó��
/// </summary>
void UCActiveSkill::Begin_Skill()
{
	CLog::Print("Skill : Begin_Skill");
	// ��ų ���� ���� 
	StartNextPhase(); 
}

void UCActiveSkill::DoAction_Skill()
{
	CLog::Print("Skill : DoAction_Skill");
	if (Index >= DoActionDatas.Num())
		return;

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCActiveSkill::End_Skill()
{
	CLog::Print("Skill : End_Skill");
	Index++;

	if(OnActionCompleted.IsBound())
		OnActionCompleted.Broadcast();

	// �����Ͱ� �� �ֳ�?
	if (Index >= DoActionDatas.Num())
		return;

	ExecutePhase(ESkillPhase::Begin_Casting);
}

void UCActiveSkill::Create_Collision()
{
	CheckNull(OwnerCharacter); 

	CLog::Print("Success Create!");
	DoActionDatas[Index].Create_SkillCollision(OwnerCharacter, HitDatas);
}

void UCActiveSkill::Create_Effect()
{
	CheckNull(OwnerCharacter);

	DoActionDatas[Index].Create_SkillEffect(OwnerCharacter);
}



