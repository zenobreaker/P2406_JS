#include "BehaviorTree/BTTask/CBTTaskNode_SpawnEntity.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

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

	ACAIController* aiController = Cast<ACAIController>(OwnerComp.GetAIOwner());
	CheckNullResult(aiController, EBTNodeResult::Failed);
	ACBoss_AI* boss = Cast<ACBoss_AI>(aiController->GetPawn());
	CheckNullResult(boss, EBTNodeResult::Failed);


	// 순찰 경로가 없다면 네비메쉬 기준으로 랜덤한 위치로 가도록 
	FVector location = boss->GetActorLocation();
	UNavigationSystemV1* navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(boss->GetWorld());
	CheckNullResult(navigation, EBTNodeResult::Failed);

	// 전용 구조체로 생성하기 
	FNavLocation point(location);
	while (true)
	{
		if (navigation->GetRandomPointInNavigableRadius(location, RandomRadius, point))
		{
			if (FVector::Dist(point.Location, boss->GetActorLocation()) >= MinRadius)
				break;
		}
	}

	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(boss->GetActorLocation(), point.Location);
	rotator.Pitch = -90;

	FTransform SpawnTransform(rotator, point.Location);
	AActor* entity = boss->GetWorld()->SpawnActorDeferred<AActor>(
		EntityClass, SpawnTransform, nullptr, nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	CheckNullResult(entity, EBTNodeResult::Failed);

	entity->FinishSpawning(SpawnTransform);

	UBlackboardComponent* blackboard = aiController->GetBlackboardComponent();
	if (!!blackboard)
	{
		aiController->ClearFocus(EAIFocusPriority::Gameplay);
		aiController->RegsterDestroyedEntity(entity);

		boss->SetBossIdleMode();

		blackboard->SetValueAsVector("TargetLocation", entity->GetActorLocation());
		blackboard->SetValueAsObject("PickupObject", entity);

	}
	return EBTNodeResult::Succeeded;
}
