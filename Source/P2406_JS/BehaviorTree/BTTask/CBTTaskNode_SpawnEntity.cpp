#include "BehaviorTree/BTTask/CBTTaskNode_SpawnEntity.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CBoss_AI.h"
#include "Characters/CAIController.h"

UCBTTaskNode_SpawnEntity::UCBTTaskNode_SpawnEntity()
{
	NodeName = "Spawn Entity";
}

EBTNodeResult::Type UCBTTaskNode_SpawnEntity::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	CheckNullResult(EntityClass, EBTNodeResult::Failed);

	AAIController* aiController = OwnerComp.GetAIOwner();
	CheckNullResult(aiController, EBTNodeResult::Failed);
	ACBoss_AI* boss = Cast<ACBoss_AI>(aiController->GetPawn());
	CheckNullResult(boss, EBTNodeResult::Failed);

	FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	AActor* entity = boss->GetWorld()->SpawnActorDeferred<AActor>(
		EntityClass, SpawnTransform, nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNullResult(entity, EBTNodeResult::Failed);

	entity->FinishSpawning(SpawnTransform);

	UBlackboardComponent* blackboard = aiController->GetBlackboardComponent();
	if (!!blackboard)
	{
		aiController->ClearFocus(EAIFocusPriority::Gameplay);
		boss->SetBossIdleMode();
		blackboard->SetValueAsVector("TargetLocation", entity->GetActorLocation());
		blackboard->SetValueAsObject("PickupObject", entity);
	}


	return EBTNodeResult::Succeeded;
}
