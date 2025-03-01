#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_DecidePattern.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_DecidePattern : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_DecidePattern();
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
