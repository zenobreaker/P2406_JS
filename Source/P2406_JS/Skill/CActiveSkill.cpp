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

	//TODO: 등록하는 기능 때문에 일반 스킬도 확장 시켜놔야할지도?
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
	
	//TODO: 스킬 타입별로 해당 내용이 변경된다.
	// 다른 페이즈로 전환 - 디폴트는 Do_Action으로 
	currentPhase = StaticCast<ESkillPhase>(StaticCast<int8>(currentPhase) + 1);
	if (SkillPhaseTable.Contains(currentPhase) == true)
		ExecutePhase(currentPhase);
	else
		StartNextPhase();	// 페이즈의 끝을 향해 
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
//		// 애니메이션 시작
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
//	case ESkillPhase::Charging:         // 추가: 충전 중
//		Begin_Charging();
//		break;
//	case ESkillPhase::WaitingForInput:  // 추가: 입력 대기 중
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
//		// 효과 처리 단계
//		break;
//	}
//}

void UCActiveSkill::DelayNextData(float InTime)
{
	if (currentDelay >= DoActionDatas[Index].HitDelay)
	{
		// 다음 데이터로 
		DoNextData();
		return;
	}

	currentDelay += InTime;
}

// 바로 다음 데이터로 보내고 그 데이터의 페이즈를 전체를 재실행한다.  
void UCActiveSkill::DoNextData()
{
	if (Index >= DoActionDatas.Num())
		return; 

	++Index; 
	// Start로 보내는 이유는 StartNextPhase 함수에서 보내기 때문
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
/// 캐스팅 처리
/// </summary>
void UCActiveSkill::Begin_Casting()
{
	FLog::Print("Skill : Begin_Casting");
	// 데이터의 함수 실행 
	DoActionDatas[Index].Begin_Casting(OwnerCharacter);
	
	//다음 페이즈 : DoCasting로 옮김 
	StartNextPhase();
}

void UCActiveSkill::DoCasting()
{
	FLog::Print("Skill : DoCasting");
	DoActionDatas[Index].DoCasting(OwnerCharacter);
	// 다음 페이즈 처리는 Tick에서 함 
}

void UCActiveSkill::End_Casting()
{
	FLog::Print("Skill : End_Casting");
	DoActionDatas[Index].End_Casting(OwnerCharacter);
	
	// 캐스팅이 끝났으므로 데이터에 처리할게 있으면 처리하라고 하고 다음 페이즈로 간다
	StartNextPhase(); 
}


/// <summary>
/// 스킬 처리
/// </summary>
void UCActiveSkill::Begin_Skill()
{
	FLog::Print("Skill : Begin_Skill");
	// 스킬 동작 시작 
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

	//TODO: 엉뚱한 로직이다. 나중에 스킬 데이터를 수정하면 여기를 수정해야한다. 
	// 데이터가 더 있나?
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



