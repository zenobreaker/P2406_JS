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

	// ���� ������ Ÿ�̸� 
	if(CurrentDelay > 0.0f )
		CurrentDelay -= DeltaSeconds;
	
	// �ൿ ���� ���¶�� �ൿ�� �������� �ʴ´�. 
	bool bCanMove = true; 
	bCanMove = behavior->GetCandMove();
	
	if (bCanMove == false)
	{
		behavior->SetWaitMode();

		return;
	}

	// Ÿ���� ������ ����
	ACharacter* target = behavior->GetTarget(); 
	if (target == nullptr )
	{
		//behavior->SetWaitMode();
		behavior->SetPatrolMode();

		return; 
	}

	// ���� ���� ���� ������ ����
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

	// �� �ܿ� �߰�
	behavior->SetApproachMode();

}

void UCBTService_Melee::RadnomActionDelay()
{
	CurrentDelay = FMath::RandRange(ActionDelay.X, ActionDelay.Y);
}

