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
    // �ִϸ��̼� ��Ÿ�ָ� �ݺ� ����ϴ� �Լ�
    UFUNCTION(BlueprintCallable, Category = "Animation")
    void PlayRepeatingMontage(UAnimInstance* AnimInstance, UAnimMontage* Montage, float PlayRate = 1.0f);

    // ��Ÿ�� ���� �� ȣ��Ǵ� �Լ�
    UFUNCTION()
    void OnMontageEnded(UAnimMontage* Montage, bool bInterrupted);
private:
    // �ִϸ��̼� ��Ÿ�� ��� Ÿ�̸�
    static FTimerHandle RepeatingTimerHandle;
};
