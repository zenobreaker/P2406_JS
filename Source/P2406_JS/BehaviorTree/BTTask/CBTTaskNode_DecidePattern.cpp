#include "BehaviorTree/BTTask/CBTTaskNode_DecidePattern.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/CAIController.h"
#include "Components/CPatternComponent.h"


UCBTTaskNode_DecidePattern::UCBTTaskNode_DecidePattern()
{
	NodeName = "Decide Pattern";
}

EBTNodeResult::Type UCBTTaskNode_DecidePattern::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	CheckNullResult(OwnerComp.GetOwner(), EBTNodeResult::Failed);
	
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(controller->GetPawn());
	CheckNullResult(pattern, EBTNodeResult::Failed);

	if (pattern->GetDecidePattern() == false)
		pattern->DecidePattern();
	
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UCBTTaskNode_DecidePattern::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Failed;
}
