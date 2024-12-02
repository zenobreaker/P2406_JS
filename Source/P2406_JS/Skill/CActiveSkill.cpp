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

	// 다른 페이즈로 전환
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
		// 애니메이션 시작
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
		// 효과 처리 단계
		break;
	}
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
void UCActiveSkill::Begin_Casting()
{
	CLog::Print("Skill : Begin_Casting");
	// 데이터의 함수 실행 
	DoActionDatas[Index].Begin_Casting(OwnerCharacter);
	
	//다음 페이즈 : DoCasting로 옮김 
	StartNextPhase();
}

void UCActiveSkill::DoCasting()
{
	CLog::Print("Skill : DoCasting");
	DoActionDatas[Index].DoCasting(OwnerCharacter);
	// 다음 페이즈 처리는 Tick에서 함 
}

void UCActiveSkill::End_Casting()
{
	CLog::Print("Skill : End_Casting");
	DoActionDatas[Index].End_Casting(OwnerCharacter);
	
	// 캐스팅이 끝났으므로 데이터에 처리할게 있으면 처리하라고 하고 다음 페이즈로 간다
	StartNextPhase(); 
}


/// <summary>
/// 스킬 처리
/// </summary>
void UCActiveSkill::Begin_Skill()
{
	CLog::Print("Skill : Begin_Skill");
	// 스킬 동작 시작 
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

	// 데이터가 더 있나?
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



