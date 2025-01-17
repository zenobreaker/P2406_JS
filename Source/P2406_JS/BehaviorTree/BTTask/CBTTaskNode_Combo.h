#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Combo.generated.h"

UCLASS()
class P2406_JS_API UCBTTaskNode_Combo : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Combo();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public: 
	UPROPERTY(EditAnywhere, Category = "Combo")
	FVector2D ComboDecideValue = FVector2D(0.0f, 1.0f);

private:
	float CurrentValue;
};
