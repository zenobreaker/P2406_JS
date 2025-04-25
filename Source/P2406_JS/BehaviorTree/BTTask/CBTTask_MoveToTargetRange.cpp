#include "BehaviorTree/BTTask/CBTTask_MoveToTargetRange.h"
#include "Global.h"

#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h" // FPathFollowingResult�� ���� �ʿ�

UCBTTask_MoveToTargetRange::UCBTTask_MoveToTargetRange()
{
	NodeName = "Move To Target Range";

	TargetKey.AddObjectFilter(this, NAME_None, AActor::StaticClass());
	RangeKey.AddFloatFilter(this, NAME_None);

	bNotifyTick = true;
	bNotifyTaskFinished = true;

	AcceptanceRadius = 0.f;
	MoveRequestID = FAIRequestID::InvalidRequest;
}

EBTNodeResult::Type UCBTTask_MoveToTargetRange::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!Target) return EBTNodeResult::Failed;

	float AttackRange = Blackboard->GetValueAsFloat(RangeKey.SelectedKeyName);
	if (AttackRange <= 0.f) return EBTNodeResult::Failed;

	const FVector ToTarget = Target->GetActorLocation() - Pawn->GetActorLocation();
	const float Distance2D = FVector::Dist2D(Pawn->GetActorLocation(), Target->GetActorLocation());

	if (Distance2D <= AttackRange)
	{
		return EBTNodeResult::Succeeded;
	}


	FVector Direction = ToTarget.GetSafeNormal2D();
	FVector MoveToLocation = Target->GetActorLocation() - Direction * (AttackRange - BufferRadius);

	if (bDebugMode)
		DrawDebugSphere(Pawn->GetWorld(), MoveToLocation, 25.f, 12, FColor::Green, false, 3.f);

	FAIMoveRequest MoveReq;
	MoveReq.SetGoalLocation(MoveToLocation);
	MoveReq.SetAcceptanceRadius(AcceptanceRadius); // ��¦ ������ ��

	FPathFollowingRequestResult Result = AIController->MoveTo(MoveReq);
	if (Result.Code == EPathFollowingRequestResult::RequestSuccessful)
	{
		MoveRequestID = Result.MoveId;
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UCBTTask_MoveToTargetRange::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !AIController->GetPathFollowingComponent())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (AIController->GetPathFollowingComponent()->GetStatus() == EPathFollowingStatus::Idle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

void UCBTTask_MoveToTargetRange::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController && MoveRequestID.IsValid())
	{
		AIController->StopMovement();
	}

	MoveRequestID = FAIRequestID::InvalidRequest;
}

FString UCBTTask_MoveToTargetRange::GetStaticDescription() const
{
	// �����Ϳ��� ��忡 ǥ�õ� ������ ��ȯ�մϴ�.
	FString Description = TEXT("Move to target's range");
	// ������ Ű �̸� ���� �߰��Ͽ� �� ���� ���� ���� ����
	// Description += FString::Printf(TEXT(" (Target: %s, Range: %s)"), *TargetKey.SelectedKeyName.ToString(), *RangeKey.SelectedKeyName.ToString());
	return Description;
}
