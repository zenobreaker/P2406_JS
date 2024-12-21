#include "Skill/CActiveSkill.h"
#include "Global.h"
#include "Components/CStatecomponent.h"
#include "GameFramework/Character.h"

void UCActiveSkill::BeginPlay(ACharacter* InOwner, const TArray<FSkillActionData>& InDoActionDatas, const TArray<FSkillHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;

	//OnActionCompleted.AddDynamic(this, &UCActiveSkill::StartNextPhase);

	FLog::Print(OwnerCharacter->GetName() + "Skill Create!");

	//TODO: ����ϴ� ��� ������ �Ϲ� ��ų�� Ȯ�� ���ѳ���������?
	RegisterSkillPhase_Normal();
}

void UCActiveSkill::Tick(float InDeltaTime)
{
	CasetingSkill(InDeltaTime);
}

void UCActiveSkill::RegisterSkillPhase_Normal()	
{
	RegisterSkillPhase(ESkillPhase::Begin_Casting,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::Begin_Casting));

	RegisterSkillPhase(ESkillPhase::Casting,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::DoCasting));

	RegisterSkillPhase(ESkillPhase::End_Casting,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::End_Casting));

	RegisterSkillPhase(ESkillPhase::Begin_Skill,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::Begin_Skill));

	RegisterSkillPhase(ESkillPhase::DoAction_Skill,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::DoAction_Skill));

	RegisterSkillPhase(ESkillPhase::End_Skill,
		TDelegate<void()>::CreateUObject(this, &UCActiveSkill::End_Skill));
}

void UCActiveSkill::RegisterSkillPhase(ESkillPhase phase, FSkillAction action)
{
	SkillPhaseTable.Add(phase, action);
}

void UCActiveSkill::ExecuteSkill()
{
	CheckNull(OwnerCharacter);
	
	// Phase -> Start 
	currentPhase = ESkillPhase::Start;

	// Data Initialize
	{
		currentCooldown = SkillInfo.CoolDown;
		Index = 0;
	}
	
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
		FLog::Print("Skill Phase All Complete");
		return; 
	}
	
	//TODO: ��ų Ÿ�Ժ��� �ش� ������ ����ȴ�.
	// �ٸ� ������� ��ȯ - ����Ʈ�� Do_Action���� 
	currentPhase = StaticCast<ESkillPhase>(StaticCast<int8>(currentPhase) + 1);
	if (SkillPhaseTable.Contains(currentPhase) == true)
		ExecutePhase(currentPhase);
	else
		StartNextPhase();	// �������� ���� ���� 
}
void UCActiveSkill::ExecutePhase(ESkillPhase phase)
{
	if (SkillPhaseTable.Contains(phase))
		SkillPhaseTable[phase].ExecuteIfBound();
}
//
//void UCActiveSkill::ExecutePhase(ESkillPhase phase)
//{
//	switch (phase)
//	{
//	case ESkillPhase::Begin_Casting:
//		Begin_Casting();
//
//		break;
//	case ESkillPhase::Casting:
//		// �ִϸ��̼� ����
//		DoCasting();
//
//		break;
//	case ESkillPhase::End_Casting:
//		End_Casting();
//
//		break;
//
//	case ESkillPhase::Begin_Skill:
//		Begin_Skill();
//
//		break;
//	case ESkillPhase::Charging:         // �߰�: ���� ��
//		Begin_Charging();
//		break;
//	case ESkillPhase::WaitingForInput:  // �߰�: �Է� ��� ��
//		Begin_WaitInput();
//
//		break; 
//	case ESkillPhase::DoAction_Skill:
//
//		DoAction_Skill();
//
//		break;
//	case ESkillPhase::End_Skill:
//		End_Skill();
//
//		break;
//	case ESkillPhase::Effect:
//		// ȿ�� ó�� �ܰ�
//		break;
//	}
//}

void UCActiveSkill::DelayNextData(float InTime)
{
	if (currentDelay >= DoActionDatas[Index].HitDelay)
	{
		// ���� �����ͷ� 
		DoNextData();
		return;
	}

	currentDelay += InTime;
}

// �ٷ� ���� �����ͷ� ������ �� �������� ����� ��ü�� ������Ѵ�.  
void UCActiveSkill::DoNextData()
{
	if (Index >= DoActionDatas.Num())
		return; 

	++Index; 
	// Start�� ������ ������ StartNextPhase �Լ����� ������ ����
	currentPhase = ESkillPhase::Start; 
}

void UCActiveSkill::Update_Cooldown(float InDeltaTime)
{
	if (currentCooldown <= 0.0f)
		return; 

	currentCooldown -= InDeltaTime; 
	/*FLog::Print(SkillInfo.SkillName + " "  + FString::SanitizeFloat(currentCooldown), 1,10.0f, FColor::Red);*/
	currentCooldown = FMath::Clamp(currentCooldown, 0.0f, SkillInfo.CoolDown);

	if (OnSkillCooldownUpdated.IsBound())
		OnSkillCooldownUpdated.Broadcast(InDeltaTime);
}

/// <summary>
/// ĳ���� ó��
/// </summary>
void UCActiveSkill::Begin_Casting()
{
	FLog::Print("Skill : Begin_Casting");
	// �������� �Լ� ���� 
	DoActionDatas[Index].Begin_Casting(OwnerCharacter);
	
	//���� ������ : DoCasting�� �ű� 
	StartNextPhase();
}

void UCActiveSkill::DoCasting()
{
	FLog::Print("Skill : DoCasting");
	DoActionDatas[Index].DoCasting(OwnerCharacter);
	// ���� ������ ó���� Tick���� �� 
}

void UCActiveSkill::End_Casting()
{
	FLog::Print("Skill : End_Casting");
	DoActionDatas[Index].End_Casting(OwnerCharacter);
	
	// ĳ������ �������Ƿ� �����Ϳ� ó���Ұ� ������ ó���϶�� �ϰ� ���� ������� ����
	StartNextPhase(); 
}


/// <summary>
/// ��ų ó��
/// </summary>
void UCActiveSkill::Begin_Skill()
{
	FLog::Print("Skill : Begin_Skill");
	// ��ų ���� ���� 
	StartNextPhase(); 
}

void UCActiveSkill::DoAction_Skill()
{
	FLog::Print("Skill : DoAction_Skill");
	if (Index >= DoActionDatas.Num())
		return;

	DoActionDatas[Index].DoAction(OwnerCharacter);
}

void UCActiveSkill::Begin_Charging()
{
}

void UCActiveSkill::Begin_WaitInput()
{
	FLog::Print("Skill Wait!");
}

void UCActiveSkill::End_Skill()
{
	FLog::Print("Skill : End_Skill");
	Index++;

	//TODO: ������ �����̴�. ���߿� ��ų �����͸� �����ϸ� ���⸦ �����ؾ��Ѵ�. 
	// �����Ͱ� �� �ֳ�?
	if (Index >= DoActionDatas.Num())
	{
		currentPhase = ESkillPhase::Finished;
		return;
	}
	
	ExecutePhase(ESkillPhase::Begin_Casting);
}

void UCActiveSkill::Create_Collision()
{
	CheckNull(OwnerCharacter); 

	DoActionDatas[Index].Create_SkillCollision(OwnerCharacter, HitDatas);
}

void UCActiveSkill::Create_Effect()
{
	CheckNull(OwnerCharacter);

	DoActionDatas[Index].Create_SkillEffect(OwnerCharacter);
}

void UCActiveSkill::OnSkillCasting()
{
}

void UCActiveSkill::OffSkillCasting()
{
}

void UCActiveSkill::OnSkillDoAction()
{
}

void UCActiveSkill::OffSkillDoAction()
{
}



