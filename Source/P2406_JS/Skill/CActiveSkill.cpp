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

	//FlowData 에 있는 것들을 기준으로 설정 
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
	// 없으면 만들 순 있어도 데이터가 없을 것 
	if (PhaseDataTable.Contains(InPhase) == false)
	{
		phase.PhaseFunction = InFunc;
		SkillPhaseTable.Emplace(InPhase, phase); 

		return;
	}

	// 기존 데이터 업데이트 
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
	// 스킬 종료 
	bIsAction = false;

	// 종료 관련한 이벤트 있으면 처리 
	DYNAMIC_EVENT_CALL(OnSkillEnded);
	//End_Skill();
	//Finish_Skill();
	// 스킬의 종료 루틴을 위해 End_Skill을 콜하고 End_Skill에서 Finish로 보내라 
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
	// 캐스팅 시작 상태에서만 캐스팅 돎
	CheckFalse(CurrentPhase == ESkillPhase::Begin_Casting);

	// ㅋㅐ스팅 완료시
	if (currentCastingTime >= SkillInfo.CastingTime)
	{
		// 델레게이트 호출하고 종료 
		DYNAMIC_EVENT_CALL(OnSkillCastingCompleted);

		return;
	}

	currentCastingTime += InTime;
}



// 값을 받으면 정해진 페이즈 값을 반환 
ESkillPhase UCActiveSkill::GetNextFlowPhase(ESkillPhase InPhase)
{
	//TODO: 다음엔 스킬 플로우 데이터를 토대로 자동완성 시키는 기능을 구현해야 겠다 

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
		// 없으면 다음 페이즈로 넘어감 
		OnChangeNextSkillPhase(InPhase);
	}
}


/// <summary>
/// 아래처럼 데이터를 전달한다. 캐릭터 데이터가 없으면 자동으로 스킬 생성시에 만든 주체자로 
/// SkillPhaseTable[ESkillPhase::Begin_Casting].PhaseDatas[0].ExecutePhase(OwnerCharacter); 
/// </summary>
void UCActiveSkill::RunSkillPhaseData(ESkillPhase InPhase, int32 InIndex, ACharacter* InCharacter)
{
	// 캐릭터 데이터가 없으면 자동으로 주체자를 할당 
	if (InCharacter == nullptr)
		InCharacter = OwnerCharacter;

	// 기본적으로 0번 인덱스가 데이터가 있다는 가정하의 진행해야함
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

	// 해당 페이즈로 강제로 보내버림 
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

