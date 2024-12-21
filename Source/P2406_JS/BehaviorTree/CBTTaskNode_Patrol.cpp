#include "BehaviorTree/CBTTaskNode_Patrol.h"
#include "Global.h"
#include "Components/CAIBehaviorComponent.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "CPatrolPath.h"


UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);


	// ���� ��ΰ� �ִٸ� �ش� ��η� �����ϵ��� 
	if (!!ai->GetPatrolPath())
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 10, 10, FColor::Green, false, 5);

		return EBTNodeResult::InProgress;
	}

	// ���� ��ΰ� ���ٸ� �׺�޽� �������� ������ ��ġ�� ������ 
	FVector location = ai->GetActorLocation();
	UNavigationSystemV1* navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	CheckNullResult(navigation, EBTNodeResult::Failed);

	// ���� ����ü�� �����ϱ� 
	FNavLocation point(location);

	// ���� �ݰ濡�� ������ ��ġ�� ���� ���´�.
	while (true)
	{
		if (navigation->GetRandomPointInNavigableRadius(location, RandomRadius, point))
			break;
	}

	// ������ ��ġ�� ��ġ�� ����
	behavior->SetPatrolLocation(location);

	if (bDebugMode)
		DrawDebugSphere(ai->GetWorld(), point.Location, 10, 10, FColor::Green, false, 5);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	FVector location = behavior->GetPatrolLocation();
	//EPathFollowingRequestResult�� AI�� MoveToLocation() �Լ��� ��ȯ ���̴�.
	EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, GoalDistance, false);

	switch (result)
	{
		case EPathFollowingRequestResult::Failed:
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

			break;
		}

		case EPathFollowingRequestResult::AlreadyAtGoal:
		{
			if (!!ai->GetPatrolPath())
				ai->GetPatrolPath()->UpdateIndex();

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

			break;
		}
	}
}
