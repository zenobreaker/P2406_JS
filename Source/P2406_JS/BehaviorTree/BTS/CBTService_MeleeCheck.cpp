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

	//// 공격할 대상을 찾는다. 
	//ACharacter* target = nullptr;
	//target = CachedBehavior->GetTarget();

	//ChangeTarget(target);

	//// 배틀 매니저에게 자신이 바라보는 타겟과 같은 타겟을 가진 대상을 검색
	//// 같은 대상에게 내가 먼저 공격 우선권을 가졌으면 걔네들을 다시 대기하라고 하게함 







	//float distance = CachedAI->GetDistanceTo(target);
	//FVector targetPosition = target->GetActorLocation();
	//// 타겟이 있으면 타겟 주변에 다른 팀원들 검사 
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

	//// ㅇㅏ래 이 시발같은 망조의 짐승은 배틀매니저라는 신문물이 해결한다. 
	//int32 myTeam_id = CachedAI->GetTeamID();
	//for (const FOverlapResult& result : results)
	//{
	//	ACEnemy_AI* other_Ai = Cast<ACEnemy_AI>(result.GetActor());
	//	if (other_Ai == nullptr)
	//		continue;
	//	// 다른 팀인지 구분 
	//	int32 other_id = other_Ai->GetTeamID();
	//	// 다른 팀은 생각안 함
	//	if (other_id != myTeam_id)
	//		continue;

	//	// 추격하는 대상도 제외 

	//	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(other_Ai);
	//	if (state == nullptr)
	//		continue;

	//	bool bCheck = true;
	//	bCheck = Calc_ThinkValue();
	//	if (bCheck)
	//		return;

	//	// 왜 추격하다가 공격타이밍 때 되돌아갈까? 
	//	// 하나라도 공격자가 있으면 자신의 상태는 Wait으로 대기한다. 
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

	//ThinkValue 내에 값이 온다면 공격자가 있어도 공격하게 한다. 
	float randValue = FMath::RandRange(0.0f, (float)ThinkValue.Y);

	// 생각한 결과가 조건과 맞아 떨어지면 그대로 수행하게 한다.
	if (randValue <= ThinkValue.X)
	{
		return true;
	}

	return false;
}

void UCBTService_MeleeCheck::Calc_WaitAndAvoidWithWeight()
{
	// WeightValue에 x값보다 작으면 wait 크면 avoid 하기로 한다. 

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
	// 새로운 타겟
	if (PrevTarget != InTarget)
	{
		PrevTarget = InTarget;

	

		
	}
	// 기존타겟과 같음 
	
	// 이전 노드에서 내가 공격 상태가 되었다면 
	if (CachedBehavior->IsActionMode())
	{
		// 배틀 매니저한테 나 공격해도 되냐고 묻는다. 
	}
}
