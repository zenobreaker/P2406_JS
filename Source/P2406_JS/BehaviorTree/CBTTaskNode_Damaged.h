#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Damaged.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_Damaged : public UBTTaskNode
{
	GENERATED_BODY()

private:

public:
	UCBTTaskNode_Damaged();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
