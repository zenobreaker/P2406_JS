#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_PlayIntroAnim.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_PlayIntroAnim : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_PlayIntroAnim();

private:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* IntroMontage;
	
	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FName CompleteIntoKey = "bCompleteIntro";

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FTimerHandle Handle;
};
