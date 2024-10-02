#include "BehaviorTree/CBTService_Range.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Range::UCBTService_Range()
{
	NodeName = "Range";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Range::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);


	if (bDrawDebug)
	{
		FVector start = ai->GetActorLocation();
		start.Z -= 25.0f;

		FVector end = start;
		end.Z += 50.0f;

		DrawDebugCylinder(ai->GetWorld(), start, end, AvoidRange, 10, FColor::Red, false, Interval);
	}

	if (state->IsDamagedMode())
	{
		behavior->SetHittedMode();

		return;
	}


	ACharacter* target = behavior->GetTarget();

	if (target == nullptr)
	{
		// ??¨¨?? ?ÄØ???? 
		controller->ClearFocus(EAIFocusPriority::Gameplay);
		behavior->SetWaitMode();

		return;
	}

	// ????? ????? 
	controller->SetFocus(target);


	float distance = ai->GetDistanceTo(target);
	if (distance < AvoidRange)
	{
		behavior->SetAvoidMode();

		return;
	}

	behavior->SetActionMode();

}
