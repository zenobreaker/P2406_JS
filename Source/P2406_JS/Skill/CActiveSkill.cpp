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

void UCActiveSkill::ExecuteSkill()
{
	CheckNull(OwnerCharacter);

	// 캐스팅 
	if(currentCastingTime > 0.0f)
		return;

	// 액션 
    OwnerCharacter->PlayAnimMontage(SkillInfo.PlayAnimMontage);

}

void UCActiveSkill::CasetingSkill(float InTime)
{

}

void UCActiveSkill::StartNextPhase()
{
    switch (currentPhase)
    {
    case ESkillPhase::Start:
        // 초기화 단계
        currentPhase = ESkillPhase::Windup;
        break;

    case ESkillPhase::Windup:
        // 애니메이션 시작
        //PlayAnimation(WindupAnimation);
        currentPhase = ESkillPhase::Execution;
        break;

    case ESkillPhase::Execution:
        // 애니메이션에서 노티파이를 통해 충돌 판정 호출
        //OwnerCharacter->PlayAnimMontage(SkillInfo);
        currentPhase = ESkillPhase::Recovery;
        break;

    case ESkillPhase::Recovery:
        // 마무리 단계
        EndSkill();
        break;
    }
}

void UCActiveSkill::EndSkill()
{

}


