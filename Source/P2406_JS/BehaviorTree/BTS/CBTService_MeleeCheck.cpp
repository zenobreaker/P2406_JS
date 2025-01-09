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

	// ������ ����� ã�´�. 
	ACharacter* target = nullptr; 
	target = CachedBehavior->GetTarget(); 
	CheckNull(target);
	
	FVector targetPosition = target->GetActorLocation();
	// Ÿ���� ������ Ÿ�� �ֺ��� �ٸ� ������ �˻� 
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

		// �ϳ��� �����ڰ� ������ �ڽ��� ���´� Wait���� ����Ѵ�. 
		if (state->IsActionMode())
		{
			CachedBehavior->SetWaitMode();

			return; 
		}
	}

}
