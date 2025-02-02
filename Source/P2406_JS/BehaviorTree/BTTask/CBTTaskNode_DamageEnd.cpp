#include "CBTTaskNode_DamageEnd.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CStateComponent.h"


UCBTTaskNode_DamageEnd::UCBTTaskNode_DamageEnd()
{
	NodeName = "Damage End";
}

EBTNodeResult::Type UCBTTaskNode_DamageEnd::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNullResult(controller, EBTNodeResult::Type::Failed);
	
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(controller->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Type::Failed);

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNullResult(behavior, EBTNodeResult::Type::Failed);

	// ������ ���°��� �������·� �����ϸ� �������� �׷��� �����Ƿ� �ʱ��
	behavior->SetNoneMode(); 

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai); 
	CheckNullResult(state, EBTNodeResult::Type::Failed);

	state->SetIdleMode();

	return EBTNodeResult::Succeeded;
}
