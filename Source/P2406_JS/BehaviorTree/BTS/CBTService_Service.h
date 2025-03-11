#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Service.generated.h"

UCLASS()
class P2406_JS_API UCBTService_Service : public UBTService
{
	GENERATED_BODY()

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
