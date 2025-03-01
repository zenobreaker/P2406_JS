#include "BehaviorTree/BTTask/CBTTaskNode_RotateToTarget.h"
#include "Global.h"

#include "Characters/CAIController_Boss.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"

UCBTTaskNode_RotateToTarget::UCBTTaskNode_RotateToTarget()
{
	NodeName = "Rotate To Target";
	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	CheckNullResult(OwnerComp.GetOwner(), EBTNodeResult::Failed);

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(controller->GetPawn());
	CheckNullResult(behavior, EBTNodeResult::Failed);

	Target = behavior->GetTarget();

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	CheckNull(OwnerComp.GetOwner());

	ACAIController_Boss* controller = Cast<ACAIController_Boss>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	if (Target != nullptr)
	{
		controller->RotateTowardsTarget(DeltaSeconds, Target);
		float rotationDiff = FMath::Abs(ai->GetActorRotation().Yaw 
			- (Target->GetActorLocation() - ai->GetActorLocation()).Rotation().Yaw);
		if (rotationDiff < RotationDiff)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return;
		}
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		
		return;
	}

	
}
