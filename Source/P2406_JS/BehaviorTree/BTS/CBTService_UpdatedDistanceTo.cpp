#include "BehaviorTree/BTS/CBTService_UpdatedDistanceTo.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Components/CAIBehaviorComponent.h"

UCBTService_UpdatedDistanceTo::UCBTService_UpdatedDistanceTo()
{
	NodeName = "UpdateDistanceToTarget";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_UpdatedDistanceTo::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	// 타겟과의 거리 제기 
	UBlackboardComponent* bb = OwnerComp.GetBlackboardComponent();
	CheckNull(bb); 

	AActor* target = Cast<AActor>(bb->GetValueAsObject(TargetKey.SelectedKeyName));
	APawn* self = OwnerComp.GetAIOwner()->GetPawn();

	if (target == nullptr || self == nullptr) return;

	const float Distance = FVector::Dist(self->GetActorLocation(), target->GetActorLocation());
	bb->SetValueAsFloat(DistanceKey.SelectedKeyName, Distance);
}
