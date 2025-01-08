#include "BehaviorTree/BTS/CBTTask_MoveTo_LookAt.h"
#include "Global.h"

#include "GameFramework/Character.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CAIBehaviorComponent.h"

UCBTTask_MoveTo_LookAt::UCBTTask_MoveTo_LookAt()
{
	NodeName = "MoveTo_LooAt";
	//bNotifyTick = true;
}

EBTNodeResult::Type UCBTTask_MoveTo_LookAt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	// Focus ¼³Á¤
	ACAIController* aiController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNullResult(aiController, EBTNodeResult::Failed); 

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(aiController->GetPawn());
	CheckNullResult(ai, EBTNodeResult::Failed);

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
	CheckNullResult(behavior, EBTNodeResult::Failed);

	ACharacter* target = behavior->GetTarget();

	if (aiController && target != nullptr)
	{
		aiController->SetFocus(target, EAIFocusPriority::Gameplay);
	}

	return Result;
}



