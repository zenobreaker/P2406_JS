#include "Animation/CAnimMontageRepeater.h"
#include "Global.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"

void UCAnimMontageRepeater::PlayRepeatingMontage(UAnimInstance* AnimInstance, UAnimMontage* Montage, float PlayRate)
{
    if (AnimInstance && Montage)
    {
        // ��Ÿ�� ����
        AnimInstance->Montage_Play(Montage, PlayRate);

        // ��Ÿ�ְ� ���� ������ �ݺ� ���
        AnimInstance->OnMontageEnded.AddDynamic(this, &UCAnimMontageRepeater::OnMontageEnded);
    }
}

void UCAnimMontageRepeater::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    
}
