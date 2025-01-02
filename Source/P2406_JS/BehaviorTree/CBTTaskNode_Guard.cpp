#include "BehaviorTree/CBTTaskNode_Guard.h"
#include "CBTTaskNode_Guard.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"

#include "Components/CGuardComponent.h"
#include "Components/CWeaponComponent.h"

UCBTTaskNode_Guard::UCBTTaskNode_Guard()
{
	NodeName = "Guard";

	bNotifyTick = true; 
}

EBTNodeResult::Type UCBTTaskNode_Guard::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCWeaponComponent* weapon = FHelpers::GetComponent <UCWeaponComponent>(ai); 
	CheckNullResult(weapon, EBTNodeResult::Failed);
	CheckTrueResult(weapon->IsUnarmedMode(), EBTNodeResult::Failed);

	UCGuardComponent* guard = FHelpers::GetComponent<UCGuardComponent>(ai);
	CheckNullResult(guard, EBTNodeResult::Failed);

	bool bCheck = false; 
	bCheck |= guard->GetCanGuard();
	CheckFalseResult(bCheck, EBTNodeResult::Failed);

	guard->StartGuard();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Guard::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());


	UCGuardComponent* Guard = FHelpers::GetComponent<UCGuardComponent>(ai);
	if (Guard == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

		return; 
	}


	if(Guard->GetGuarding() == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	else 
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);

}

EBTNodeResult::Type UCBTTaskNode_Guard::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCGuardComponent* guard = FHelpers::GetComponent<UCGuardComponent>(ai);
	CheckNullResult(guard, EBTNodeResult::Failed);

	guard->StopGuard();

	return EBTNodeResult::Succeeded;
}
