#include "BehaviorTree/BTTask/CBTTaskNode_CancelCurrentPattern.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Components/CPatternComponent.h"

UCBTTaskNode_CancelCurrentPattern::UCBTTaskNode_CancelCurrentPattern()
{
	NodeName = "CancelCurrentPattern";
}

EBTNodeResult::Type UCBTTaskNode_CancelCurrentPattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CheckNullResult(OwnerComp.GetOwner(), EBTNodeResult::Failed);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(controller->GetPawn());
	CheckNullResult(pattern, EBTNodeResult::Failed);

	pattern->CancelPattern(); 

	return EBTNodeResult::Succeeded;
}
