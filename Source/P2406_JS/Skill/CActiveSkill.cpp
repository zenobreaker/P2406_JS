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

	// ĳ���� 
	if(currentCastingTime > 0.0f)
		return;

	// �׼� 
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
        // �ʱ�ȭ �ܰ�
        currentPhase = ESkillPhase::Windup;
        break;

    case ESkillPhase::Windup:
        // �ִϸ��̼� ����
        //PlayAnimation(WindupAnimation);
        currentPhase = ESkillPhase::Execution;
        break;

    case ESkillPhase::Execution:
        // �ִϸ��̼ǿ��� ��Ƽ���̸� ���� �浹 ���� ȣ��
        //OwnerCharacter->PlayAnimMontage(SkillInfo);
        currentPhase = ESkillPhase::Recovery;
        break;

    case ESkillPhase::Recovery:
        // ������ �ܰ�
        EndSkill();
        break;
    }
}

void UCActiveSkill::EndSkill()
{

}


