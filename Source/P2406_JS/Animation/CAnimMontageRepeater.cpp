#include "Animation/CAnimMontageRepeater.h"
#include "Global.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimInstance.h"

void UCAnimMontageRepeater::PlayRepeatingMontage(UAnimInstance* AnimInstance, UAnimMontage* Montage, float PlayRate)
{
    if (AnimInstance && Montage)
    {
        // 몽타주 시작
        AnimInstance->Montage_Play(Montage, PlayRate);

        // 몽타주가 끝날 때마다 반복 재생
        AnimInstance->OnMontageEnded.AddDynamic(this, &UCAnimMontageRepeater::OnMontageEnded);
    }
}

void UCAnimMontageRepeater::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
    
}
