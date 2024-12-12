#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "UObject/NoExportTypes.h"
#include "CAnimMontageRepeater.generated.h"


UCLASS()
class P2406_JS_API UCAnimMontageRepeater : public UObject
{
	GENERATED_BODY()
	
public:
    // 애니메이션 몽타주를 반복 재생하는 함수
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void PlayRepeatingMontage(UAnimInstance* AnimInstance, UAnimMontage* Montage, float PlayRate = 1.0f);

    // 몽타주 종료 시 호출되는 함수
    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
private:
    // 애니메이션 몽타주 재생 타이머
    static FTimerHandle RepeatingTimerHandle;
};
