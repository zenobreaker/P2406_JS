#include "BehaviorTree/BTS/CBTService_MeleeCheck.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_MeleeCheck::UCBTService_MeleeCheck()
{
	NodeName = "MeleeCheck";
}

void UCBTService_MeleeCheck::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	CheckNull(CachedController);

	CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());
	CheckNull(CachedAI);

	CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
	CachedState = FHelpers::GetComponent<UCStateComponent>(CachedAI);
}

void UCBTService_MeleeCheck::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CheckNull(CachedBehavior); 

	// 공격할 대상을 찾는다. 
	ACharacter* target = nullptr; 
	target = CachedBehavior->GetTarget(); 
	CheckNull(target);
	
	FVector targetPosition = target->GetActorLocation();
	// 타겟이 있으면 타겟 주변에 다른 팀원들 검사 
	TArray<FOverlapResult> results; 

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CachedAI);

	bool bOverlapped = GetWorld()->OverlapMultiByChannel(
		results,
		targetPosition,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DetectionRadius),
		QueryParams
	);

	CheckFalse(bOverlapped);

	for (const FOverlapResult& result : results)
	{
		ACEnemy_AI* enemyAi = Cast<ACEnemy_AI>(result.GetActor());
		if (enemyAi == nullptr)
			continue; 

		UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(enemyAi);
		if (state == nullptr)
			continue; 

		// 하나라도 공격자가 있으면 자신의 상태는 Wait으로 대기한다. 
		if (state->IsActionMode())
		{
			CachedBehavior->SetWaitMode();

			return; 
		}
	}

}
