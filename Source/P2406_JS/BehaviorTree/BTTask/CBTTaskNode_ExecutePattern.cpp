#include "BehaviorTree/BTTask/CBTTaskNode_ExecutePattern.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Components/CPatternComponent.h"

UCBTTaskNode_ExecutePattern::UCBTTaskNode_ExecutePattern()
{
	NodeName = "Execute Pattern";

	bNotifyTick = true; 
}

EBTNodeResult::Type UCBTTaskNode_ExecutePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	CheckNullResult(OwnerComp.GetOwner(), EBTNodeResult::Failed);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(controller->GetPawn());
	CheckNullResult(pattern, EBTNodeResult::Failed);

	if (pattern->GetDecidePattern() == false || pattern->IsExecutePattern() == true)
		return EBTNodeResult::Failed;
	
	controller->ClearFocus(EAIFocusPriority::Gameplay);

	pattern->ExecutePattern();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_ExecutePattern::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller); 
	UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(controller->GetPawn());
	CheckNull(pattern);

	if(pattern->IsExecutePattern() == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UCBTTaskNode_ExecutePattern::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
