#include "CBTTaskNode_Damaged.h"
#include "Global.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CAIController.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CConditionComponent.h"
#include "Weapons/CEquipment.h"


UCBTTaskNode_Damaged::UCBTTaskNode_Damaged()
{
	bNotifyTick = true;

	NodeName = "Damaged";
}

EBTNodeResult::Type UCBTTaskNode_Damaged::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Failed);
	controller->StopMovement();
	// ��Ŀ�� �� �������� ���� 
	controller->ClearFocus(EAIFocusPriority::Gameplay);
	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Damaged::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	UCConditionComponent* condition = FHelpers::GetComponent<UCConditionComponent>(ai);
	if (state->IsDamagedMode() == false)
	{
		// �ٽ� ���� Ÿ������ ���� .
		UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai); 
		if (!!behavior)
		{
			ACharacter* target = behavior->GetTarget();
			if (!!target && condition != nullptr && (condition->GetAirborneCondition() == false &&
				condition->GetDownCondition() == false ))
			{
				controller->SetFocus(target);
			}
		}

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
