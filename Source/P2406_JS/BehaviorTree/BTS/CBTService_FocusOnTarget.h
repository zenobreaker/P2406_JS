#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_FocusOnTarget.generated.h"


UCLASS()
class P2406_JS_API UCBTService_FocusOnTarget : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_FocusOnTarget();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
