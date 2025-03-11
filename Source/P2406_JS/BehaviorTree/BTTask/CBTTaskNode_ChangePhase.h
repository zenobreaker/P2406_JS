#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "CBTTaskNode_ChangePhase.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_ChangePhase : public UBTTaskNode
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage; 

	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f; 

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector PhaseKey;

	UPROPERTY(EditAnywhere)
	float CompleteTime = -1.0f; 

public:
	UCBTTaskNode_ChangePhase(); 

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FTimerHandle Handle;
};
