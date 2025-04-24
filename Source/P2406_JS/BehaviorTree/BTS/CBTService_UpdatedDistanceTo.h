#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_UpdatedDistanceTo.generated.h"

UCLASS()
class P2406_JS_API UCBTService_UpdatedDistanceTo : public UBTService
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Distance",  meta=(AllowedTypes = "float"))
	FBlackboardKeySelector DistanceKey; 

	UPROPERTY(EditAnywhere, Category = "Distance",  meta=(AllowedTypes = "object"))
	FBlackboardKeySelector TargetKey;

public:
	UCBTService_UpdatedDistanceTo(); 

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
