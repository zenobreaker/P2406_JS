#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "CBTTask_MoveTo_LookAt.generated.h"


UCLASS()
class P2406_JS_API UCBTTask_MoveTo_LookAt : public UBTTask_MoveTo
{
	GENERATED_BODY()


public:
	UCBTTask_MoveTo_LookAt();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
