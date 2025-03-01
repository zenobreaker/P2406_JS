#include "BehaviorTree/BTTask/CBTTaskNode_ExecutePattern.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Components/CPatternComponent.h"

UCBTTaskNode_ExecutePattern::UCBTTaskNode_ExecutePattern()
{
	NodeName = "Execute Pattern";
}

EBTNodeResult::Type UCBTTaskNode_ExecutePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CheckNullResult(OwnerComp.GetOwner(), EBTNodeResult::Failed);
	

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(controller->GetPawn());
	CheckNullResult(pattern, EBTNodeResult::Failed);
	pattern->ExecutePattern();

	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UCBTTaskNode_ExecutePattern::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}
