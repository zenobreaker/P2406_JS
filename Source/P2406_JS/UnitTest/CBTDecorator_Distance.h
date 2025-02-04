#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_Distance.generated.h"


UCLASS()
class P2406_JS_API UCBTDecorator_Distance : public UBTDecorator
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	float MaxDistance = 100.0f; 

public:
	UCBTDecorator_Distance();


	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
