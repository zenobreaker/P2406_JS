#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_PickupObject.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_PickupObject : public UBTTaskNode
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* IntroMontage;
	
	UPROPERTY(EditAnywhere)
	float PlayRate = 1.0f;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BoardKey;

public:
	UCBTTaskNode_PickupObject(); 

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	void SetBlackboardValue(class UBlackboardComponent* InBlackboard);

private:
	FTimerHandle Handle;
};