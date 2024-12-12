#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f; 
	RandomDeviation = 0.0f; 
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	UCAIBehaviorComponent* behavior = CHelpers::GetComponent<UCAIBehaviorComponent>(ai);


	// 타겟이 없으면 순찰
	ACharacter* target = behavior->GetTarget(); 
	if (target == nullptr)
	{
		//behavior->SetWaitMode();
		behavior->SetPatrolMode();

		return; 
	}

	// 범위 내에 적이 잇으면 공격
	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange)
	{ 
		UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(ai);

		if (state->IsIdleMode() == false)
			return;

		behavior->SetActionMode();

		return; 
	}

	// 이 외엔 추격
	behavior->SetApproachMode();

}
