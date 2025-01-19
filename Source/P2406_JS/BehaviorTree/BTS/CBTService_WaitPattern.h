#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_WaitPattern.generated.h"


UCLASS()
class P2406_JS_API UCBTService_WaitPattern : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_WaitPattern(); 

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
};
