#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_CancelCurrentPattern.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_CancelCurrentPattern : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_CancelCurrentPattern(); 

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
