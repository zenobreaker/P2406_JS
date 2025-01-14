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
	CheckTrue(CachedBehavior->IsApproachMode());
	CheckTrue(CachedBehavior->IsDeadMode());

	// ������ ����� ã�´�. 
	ACharacter* target = nullptr; 
	target = CachedBehavior->GetTarget(); 
	CheckNull(target);
	
	FVector targetPosition = target->GetActorLocation();
	// Ÿ���� ������ Ÿ�� �ֺ��� �ٸ� ������ �˻� 
	TArray<FOverlapResult> results; 

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(CachedAI);
	QueryParams.AddIgnoredActor(target);

	bool bOverlapped = GetWorld()->OverlapMultiByChannel(
		results,
		targetPosition,
		FQuat::Identity,
		ECC_Pawn,
		FCollisionShape::MakeSphere(DetectionRadius),
		QueryParams
	);

	CheckFalse(bOverlapped);

	int32 myTeam_id = CachedAI->GetTeamID();
	for (const FOverlapResult& result : results)
	{
		ACEnemy_AI* other_Ai = Cast<ACEnemy_AI>(result.GetActor());
		if (other_Ai == nullptr)
			continue; 
		// �ٸ� ������ ���� 
		int32 other_id = other_Ai->GetTeamID();
		// �ٸ� ���� ������ ��
		if (other_id != myTeam_id)
			continue; 

		UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(other_Ai);
		if (state == nullptr)
			continue; 

		CheckTrue(Calc_ThinkValue());
		CheckTrue(CachedBehavior->IsActionMode());
		
		
		// �ϳ��� �����ڰ� ������ �ڽ��� ���´� Wait���� ����Ѵ�. 
		if (state->IsActionMode())
		{
			FLog::Log("who are ? Attack" + other_Ai->GetName());
			CachedBehavior->SetWaitMode();

			return; 
		}
	}

}

bool UCBTService_MeleeCheck::Calc_ThinkValue()
{
	if (ThinkValue.X <= 0.0f)
		return false; 

	//ThinkValue ���� ���� �´ٸ� �����ڰ� �־ �����ϰ� �Ѵ�. 
	float randValue = FMath::RandRange(0.0f, (float)ThinkValue.Y);

	// ������ ����� ���ǰ� �¾� �������� �״�� �����ϰ� �Ѵ�.
	if (randValue <= ThinkValue.X)
	{
		return true;
	}

	return false;
}
