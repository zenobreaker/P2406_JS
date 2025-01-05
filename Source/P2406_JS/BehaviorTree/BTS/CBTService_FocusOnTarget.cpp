#include "BehaviorTree/BTS/CBTService_FocusOnTarget.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CAIBehaviorComponent.h"

UCBTService_FocusOnTarget::UCBTService_FocusOnTarget()
{
	NodeName = "FoucsOnTarget";
	Interval = 0.1f;
}

void UCBTService_FocusOnTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai);

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNull(behavior);

	// Å¸°Ù °Ë»ç 
	ACharacter* target = nullptr;
	target = behavior->GetTarget();
	controller->SetFocus(target);
}
