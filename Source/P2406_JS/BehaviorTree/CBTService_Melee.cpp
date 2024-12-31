#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/IGuardable.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	ACAIController* controller = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!controller)
	{
		CachedAI = Cast<ACEnemy_AI>(controller->GetPawn());

		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
		CachedState = FHelpers::GetComponent<UCStateComponent>(CachedAI);
		CachedGuardable = Cast<IIGuardable>(CachedAI);
	}
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	// 공격 딜레이 타이머 
	if (CurrentDelay > 0.0f)
		CurrentDelay -= DeltaSeconds;

	// 행동 제약 상태라면 행동을 개시하지 않는다. 
	CheckTrue(Tick_CheckWait());

	// 타겟이 없으면 순찰
	ACharacter* target = nullptr;
	CheckTrue(Tick_CheckPatrol(&target));

	// 범위 내에 적이 있으면 공격
	CheckTrue(Tick_CheckAttack(target));

	// 가드가 가능하면 가드 하기
	CheckTrue(Tick_CheckGuard(target));

	// 이 외엔 추격
	CheckTrue(Tick_CheckApproach());

}

void UCBTService_Melee::RadnomActionDelay()
{
	CurrentDelay = FMath::RandRange(ActionDelay.X, ActionDelay.Y);
}



bool UCBTService_Melee::Tick_CheckWait() const
{
	CheckNullResult(CachedBehavior, false);

	bool bCanMove = true;
	bCanMove = CachedBehavior->GetCandMove();
	if (bCanMove == false)
	{
		CachedBehavior->SetWaitMode();

		return true;
	}

	return false;
}

bool UCBTService_Melee::Tick_CheckPatrol(ACharacter** OutTarget) const
{
	CheckNullResult(CachedBehavior, false);

	*OutTarget = CachedBehavior->GetTarget();
	if (*OutTarget == nullptr)
	{
		//behavior->SetWaitMode();
		CachedBehavior->SetPatrolMode();

		return true;
	}

	return false;
}

bool UCBTService_Melee::Tick_CheckAttack(const ACharacter* InTarget)
{
	CheckNullResult(CachedBehavior, false);
	CheckNullResult(CachedState, false);
	CheckNullResult(CachedAI, false);
	CheckNullResult(InTarget, false);

	float distance = CachedAI->GetDistanceTo(InTarget);
	if (distance < ActionRange && CurrentDelay <= 0.0f)
	{

		if (CachedState->IsIdleMode() == false)
			return false;

		CachedBehavior->SetActionMode();

		RadnomActionDelay();

		return true;
	}

	return false;
}

bool UCBTService_Melee::Tick_CheckGuard(const ACharacter* InTarget) const
{
	CheckNullResult(CachedBehavior, false);
	CheckNullResult(CachedState, false);
	CheckNullResult(CachedAI, false);
	CheckNullResult(InTarget, false);

	bool bGuardale = false;
	if (CachedGuardable)
	{
		bGuardale = true;
	}

	if (bGuardale == false)
	{
		return false;
	}


	float distance = CachedAI->GetDistanceTo(InTarget);

	if (CachedState->IsIdleMode() == false)
		return false;

	// 적이 감지되고 상대가 나보다 거리가 멀면 일단 가드 올린다.
	if (distance > ActionRange)
	{
		CachedBehavior->SetGuardMode();
		CachedState->SetGuardMode();

		return true;
	}

	return false;

}

bool UCBTService_Melee::Tick_CheckApproach() const
{
	CheckNullResult(CachedBehavior, false);

	CachedBehavior->SetApproachMode();

	return true;
}
