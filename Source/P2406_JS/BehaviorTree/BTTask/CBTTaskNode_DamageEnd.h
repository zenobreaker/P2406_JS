#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_DamageEnd.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_DamageEnd : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_DamageEnd(); 


protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
