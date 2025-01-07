#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "CBTDecorator_CheckMoveValid.generated.h"


UCLASS()
class P2406_JS_API UCBTDecorator_CheckMoveValid : public UBTDecorator
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector TargetLocationKey; 

public:
	UCBTDecorator_CheckMoveValid();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	bool CheckPathValidity(class ACAIController& InController, FVector InLocation) const;
};
