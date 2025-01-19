#include "BehaviorTree/BTS/CBTService_WaitPattern.h"
#include "Global.h"

#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"

#include "Components/CAIBehaviorComponent.h"

UCBTService_WaitPattern::UCBTService_WaitPattern()
{
}

void UCBTService_WaitPattern::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(controller); 

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNull(ai); 

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai); 
	CheckNull(behavior); 

	CheckFalse(behavior->IsWaitMode());
	// TODO: 이거 나중에 해요 

}
