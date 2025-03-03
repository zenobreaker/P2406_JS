#include "Skill/CActiveSkill.h"
#include "Global.h"
#include "Components/CStatecomponent.h"
#include "GameFramework/Character.h"
#include "Animation/AnimInstance.h"

#include "Skill/CSkillEntity.h"

void UCActiveSkill::BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData)
{
	OwnerCharacter = InOwner;

	//FLog::Print(OwnerCharacter->GetName() + "Skill Create!");
	SkillFlowData = InFlowData;

	PhaseDataTable.Empty();
	for (FSkillPhaseData& data : SkillFlowData.PhaseDatas)
	{
		if (PhaseDataTable.Contains(data.PhaseType))
			PhaseDataTable[data.PhaseType].Add(data);
		else
		{
			PhaseDataTable.Add(data.PhaseType, TArray<FSkillPhaseData>());
			PhaseDataTable[data.PhaseType].Add(data);
		}
	}
	DefineSkillPhases();
	PhaseDataTable.Empty();
}

void UCActiveSkill::Tick(float InDeltaTime)
{
	CastingSkill(InDeltaTime);
}

TFunction<void()> UCActiveSkill::GetPhaseFunction(ESkillPhase InPhase)
{
	TFunction<void()> func;

	switch (InPhase)
	{
	case ESkillPhase::Start:
		func = [this]() { Start_Skill(); };
		break;
	case ESkillPhase::Begin_Casting:
		func = [this]() { Begin_Casting(); };
		break;
	case ESkillPhase::End_Casting:
		func = [this]() { End_Casting(); };
		break;
	case ESkillPhase::Begin_Skill:
		func = [this]() { Begin_Skill(); };
		break;
	case ESkillPhase::End_Skill:
		func = [this]() { End_Skill(); };
		break;
	case ESkillPhase::Create_Effect:
		func = [this]() { Create_SkillEffect(); };
		break;
	case ESkillPhase::Create_Collision:
		func = [this]() { Create_Collision(); };
		break;
	case ESkillPhase::Finished:
		break;
	default:
		func = []() {};
		break;
	}


	return func;
}

void UCActiveSkill::SetupDefaultSkillPhase()
{
	SkillPhaseTable.Empty();

	SkillPhaseTable.Add({ ESkillPhase::Start, FSkillPhase() });
	SkillPhaseTable.Add({ ESkillPhase::Begin_Casting, FSkillPhase() });
	SkillPhaseTable.Add({ ESkillPhase::End_Casting, FSkillPhase() });
	SkillPhaseTable.Add({ ESkillPhase::Begin_Skill, FSkillPhase() });
	SkillPhaseTable.Add({ ESkillPhase::End_Skill, FSkillPhase() });
	SkillPhaseTable.Add({ ESkillPhase::Finished, FSkillPhase() });

	//FlowData �� �ִ� �͵��� �������� ���� 
	for (FSkillPhaseData& phase : SkillFlowData.PhaseDatas)
	{
		SkillPhaseTable.Emplace(phase.PhaseType, FSkillPhase(PhaseDataTable[phase.PhaseType], GetPhaseFunction(phase.PhaseType)));
	}
}


void UCActiveSkill::AssignSkillPhase(
	ESkillPhase InPhase, 
	TFunction<void()> InFunc)
{
	UCActiveSkill::FSkillPhase phase;
	// ������ ���� �� �־ �����Ͱ� ���� �� 
	if (PhaseDataTable.Contains(InPhase) == false)
	{
		phase.PhaseFunction = InFunc;
		SkillPhaseTable.Emplace(InPhase, phase); 

		return;
	}

	// ���� ������ ������Ʈ 
	phase.PhaseDatas = PhaseDataTable[InPhase];
	phase.PhaseFunction = InFunc;
	SkillPhaseTable.Emplace(InPhase, phase); 
}


void UCActiveSkill::ExecuteSkill()
{
	CheckNull(OwnerCharacter);
	CheckTrue(bIsAction);
	
	bIsAction = true; 
	currentCooldown = SkillInfo.CoolDown;

	ExecutePhase(ESkillPhase::Start);
}
void UCActiveSkill::EndSkill()
{
	// ��ų ���� 
	bIsAction = false;

	// ���� ������ �̺�Ʈ ������ ó�� 
	DYNAMIC_EVENT_CALL(OnSkillEnded);
	//End_Skill();
	//Finish_Skill();
	// ��ų�� ���� ��ƾ�� ���� End_Skill�� ���ϰ� End_Skill���� Finish�� ������ 
	ExecutePhase(ESkillPhase::End_Skill);
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

void UCActiveSkill::CastingSkill(float InTime)
{
	// ĳ���� ���� ���¿����� ĳ���� ��
	CheckFalse(CurrentPhase == ESkillPhase::Begin_Casting);

	// �������� �Ϸ��
	if (currentCastingTime >= SkillInfo.CastingTime)
	{
		// ��������Ʈ ȣ���ϰ� ���� 
		DYNAMIC_EVENT_CALL(OnSkillCastingCompleted);

		return;
	}

	currentCastingTime += InTime;
}



// ���� ������ ������ ������ ���� ��ȯ 
ESkillPhase UCActiveSkill::GetNextFlowPhase(ESkillPhase InPhase)
{
	//TODO: ������ ��ų �÷ο� �����͸� ���� �ڵ��ϼ� ��Ű�� ����� �����ؾ� �ڴ� 

	switch (InPhase)
	{
	case ESkillPhase::Start:
		return  ESkillPhase::Begin_Casting;
	case ESkillPhase::Begin_Casting:
		return  ESkillPhase::End_Casting;
	case ESkillPhase::End_Casting:
		return  ESkillPhase::Begin_Skill;
	case ESkillPhase::Begin_Skill:
		return ESkillPhase::End_Skill;
	case ESkillPhase::End_Skill:
		return ESkillPhase::Finished;
	}

	return ESkillPhase::Max;
}



void UCActiveSkill::ExecutePhase(ESkillPhase InPhase)
{
	CheckTrue(ESkillPhase::Max == InPhase);
	CheckFalse(SkillPhaseTable.Num() > 0);

	CurrentPhase = InPhase;
	
	if(InPhase == ESkillPhase::Finished)
	{
		Finish_Skill();

		return;
	}

	if (SkillPhaseTable.Contains(InPhase) && SkillPhaseTable[InPhase].PhaseFunction)
	{
		SkillPhaseTable[InPhase].PhaseFunction();
	}
	else
	{
		// ������ ���� ������� �Ѿ 
		OnChangeNextSkillPhase(InPhase);
	}
}


/// <summary>
/// �Ʒ�ó�� �����͸� �����Ѵ�. ĳ���� �����Ͱ� ������ �ڵ����� ��ų �����ÿ� ���� ��ü�ڷ� 
/// SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas[0].ExecutePhase(OwnerCharacter); 
/// </summary>
void UCActiveSkill::RunSkillPhaseData(ESkillPhase InPhase, int32 InIndex, ACharacter* InCharacter)
{
	// ĳ���� �����Ͱ� ������ �ڵ����� ��ü�ڸ� �Ҵ� 
	if (InCharacter == nullptr)
		InCharacter = OwnerCharacter;

	// �⺻������ 0�� �ε����� �����Ͱ� �ִٴ� �������� �����ؾ���
	if (SkillPhaseTable[InPhase].PhaseDatas.Num() > 0)
		SkillPhaseTable[InPhase].PhaseDatas[InIndex].ExecutePhase(InCharacter);
}


void UCActiveSkill::OnChangeNextSkillPhase()
{
	OnChangeNextSkillPhase(CurrentPhase);
}

void UCActiveSkill::OnChangeNextSkillPhase(ESkillPhase InPhase)
{
	ESkillPhase next = GetNextFlowPhase(InPhase);

	// �ش� ������� ������ �������� 
	ExecutePhase(next);
}

void UCActiveSkill::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (bInterrupted)
	{
		FLog::Log("Montage Interrupted End ");
	}


	FLog::Log("Montage End Test");
}

void UCActiveSkill::OnActivated_Collision()
{
	CheckNull(SkillEntity);

	SkillEntity->ActivateCollision();
}

void UCActiveSkill::OnDeactivated_Collision()
{
	CheckNull(SkillEntity);

	SkillEntity->DeactivateCollision();
}

