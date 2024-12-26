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
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);

	// 공격 딜레이 타이머 
	if(CurrentDelay > 0.0f )
		CurrentDelay -= DeltaSeconds;
	
	// 행동 제약 상태라면 행동을 개시하지 않는다. 
	bool bCanMove = true; 
	bCanMove = behavior->GetCandMove();
	
	if (bCanMove == false)
	{
		behavior->SetWaitMode();

		return;
	}

	// 타겟이 없으면 순찰
	ACharacter* target = behavior->GetTarget(); 
	if (target == nullptr )
	{
		//behavior->SetWaitMode();
		behavior->SetPatrolMode();

		return; 
	}

	// 범위 내에 적이 잇으면 공격
	float distance = ai->GetDistanceTo(target);
	if (distance < ActionRange && CurrentDelay <= 0.0f)
	{ 
		UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);

		if (state->IsIdleMode() == false)
			return;

		behavior->SetActionMode();

		RadnomActionDelay();

		return; 
	}

	// 이 외엔 추격
	behavior->SetApproachMode();

}

void UCBTService_Melee::RadnomActionDelay()
{
	CurrentDelay = FMath::RandRange(ActionDelay.X, ActionDelay.Y);
}

