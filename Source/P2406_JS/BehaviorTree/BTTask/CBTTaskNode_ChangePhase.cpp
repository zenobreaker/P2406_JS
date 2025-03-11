#include "BehaviorTree/BTTask/CBTTaskNode_ChangePhase.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"

UCBTTaskNode_ChangePhase::UCBTTaskNode_ChangePhase()
{
	NodeName = "ChangePhase";
}

EBTNodeResult::Type UCBTTaskNode_ChangePhase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (controller == nullptr) return EBTNodeResult::Failed;

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	if (ai == nullptr) return EBTNodeResult::Failed;
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	if (behavior == nullptr) return EBTNodeResult::Failed;

	UBlackboardComponent* blackboard = behavior->GetBlackboard();
	if (blackboard == nullptr) return EBTNodeResult::Failed;

	
	if (Montage != nullptr)
		ai->PlayAnimMontage(Montage); 
	
	int32 currentPhase = behavior->GetCurrentPhase();
	behavior->SetLastPhase(currentPhase);

	if (CompleteTime > 0)
	{
		ai->GetWorldTimerManager().SetTimer(Handle, FTimerDelegate::CreateLambda([ai, blackboard, &OwnerComp, this]()
			{
				/*if (blackboard != nullptr)
					blackboard->SetValueAsBool(CompleteIntoKey, true);*/
				

				if (ai != nullptr)
					ai->GetWorldTimerManager().ClearTimer(Handle);

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			}), CompleteTime, false);


		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Succeeded;
}
