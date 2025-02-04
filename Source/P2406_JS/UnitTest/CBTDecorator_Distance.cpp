#include "UnitTest/CBTDecorator_Distance.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTDecorator_Distance::UCBTDecorator_Distance()
{
	NodeName = "Decorator_Distance";
}

bool UCBTDecorator_Distance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{

	ACAIController* AIController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	if (AIController == nullptr)
		return false; 

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(AIController->GetPawn());
	CheckNullResult(ai, false);

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNullResult(behavior, false);

	ACharacter* target =  behavior->GetTarget();
	CheckNullResult(target, false);

	float find_dist = FVector::Distance(ai->GetActorLocation(), target->GetActorLocation());

	return find_dist <= MaxDistance;
}
