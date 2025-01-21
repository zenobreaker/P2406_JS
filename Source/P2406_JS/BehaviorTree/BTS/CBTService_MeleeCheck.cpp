#include "BehaviorTree/BTS/CBTService_MeleeCheck.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

#include "GameInstances/CBattleManager.h"
#include "GameInstances/CGameInstance.h" 

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
	CheckTrue(CachedBehavior->IsDeadMode());

	CheckTrue(CachedBehavior->IsActionMode());

	//// ������ ����� ã�´�. 
	//ACharacter* target = nullptr;
	//target = CachedBehavior->GetTarget();

	//ChangeTarget(target);

	//// ��Ʋ �Ŵ������� �ڽ��� �ٶ󺸴� Ÿ�ٰ� ���� Ÿ���� ���� ����� �˻�
	//// ���� ��󿡰� ���� ���� ���� �켱���� �������� �³׵��� �ٽ� ����϶�� �ϰ��� 







	//float distance = CachedAI->GetDistanceTo(target);
	//FVector targetPosition = target->GetActorLocation();
	//// Ÿ���� ������ Ÿ�� �ֺ��� �ٸ� ������ �˻� 
	//TArray<FOverlapResult> results;

	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(CachedAI);
	//QueryParams.AddIgnoredActor(target);

	//bool bOverlapped = GetWorld()->OverlapMultiByChannel(
	//	results,
	//	targetPosition,
	//	FQuat::Identity,
	//	ECC_Pawn,
	//	FCollisionShape::MakeSphere(distance),
	//	QueryParams
	//);

	//CheckFalse(bOverlapped);

	//// ������ �� �ù߰��� ������ ������ ��Ʋ�Ŵ������ �Ź����� �ذ��Ѵ�. 
	//int32 myTeam_id = CachedAI->GetTeamID();
	//for (const FOverlapResult& result : results)
	//{
	//	ACEnemy_AI* other_Ai = Cast<ACEnemy_AI>(result.GetActor());
	//	if (other_Ai == nullptr)
	//		continue;
	//	// �ٸ� ������ ���� 
	//	int32 other_id = other_Ai->GetTeamID();
	//	// �ٸ� ���� ������ ��
	//	if (other_id != myTeam_id)
	//		continue;

	//	// �߰��ϴ� ��� ���� 

	//	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(other_Ai);
	//	if (state == nullptr)
	//		continue;

	//	bool bCheck = true;
	//	bCheck = Calc_ThinkValue();
	//	if (bCheck)
	//		return;

	//	// �� �߰��ϴٰ� ����Ÿ�̹� �� �ǵ��ư���? 
	//	// �ϳ��� �����ڰ� ������ �ڽ��� ���´� Wait���� ����Ѵ�. 
	//	if (state->IsActionMode())
	//	{
	//		//FLog::Log("who are ? Attack" + other_Ai->GetName());
	//		Calc_WaitAndAvoidWithWeight();

	//		return;
	//	}
	//}

}

bool UCBTService_MeleeCheck::Calc_ThinkValue()
{
	CheckTrueResult(ThinkValue.X <= 0.0f, false);

	//ThinkValue ���� ���� �´ٸ� �����ڰ� �־ �����ϰ� �Ѵ�. 
	float randValue = FMath::RandRange(0.0f, (float)ThinkValue.Y);

	// ������ ����� ���ǰ� �¾� �������� �״�� �����ϰ� �Ѵ�.
	if (randValue <= ThinkValue.X)
	{
		return true;
	}

	return false;
}

void UCBTService_MeleeCheck::Calc_WaitAndAvoidWithWeight()
{
	// WeightValue�� x������ ������ wait ũ�� avoid �ϱ�� �Ѵ�. 

	float randWeight = FMath::RandRange(0.0f, (float)WeightValue.Y);

	if (randWeight <= WeightValue.X)
	{
		FLog::Print(CachedAI->GetName() + " Wait call!!");
		CachedBehavior->SetWaitMode();

		return;
	}

}

void UCBTService_MeleeCheck::ChangeTarget(ACharacter* InTarget)
{
	// ���ο� Ÿ��
	if (PrevTarget != InTarget)
	{
		PrevTarget = InTarget;

	

		
	}
	// ����Ÿ�ٰ� ���� 
	
	// ���� ��忡�� ���� ���� ���°� �Ǿ��ٸ� 
	if (CachedBehavior->IsActionMode())
	{
		// ��Ʋ �Ŵ������� �� �����ص� �ǳİ� ���´�. 
	}
}
