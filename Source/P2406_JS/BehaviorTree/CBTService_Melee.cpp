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
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();

	// 타겟이 없으면 순찰
	bool bPatrol = Tick_CheckPatrol(target);
	CheckTrue(bPatrol);

	//TODO: 가드가 가능하면 가드 하기

	// 범위 내에 적이 있으면 공격
	bool bAttack = Tick_CheckAttack(target);
	CheckTrue(bAttack);

	// 적이 있고 공격할 수 있을 때 공격 범위 밖이면 공격하러 감
	bool bApproach = Tick_CheckApproach(target);
	CheckTrue(bApproach);

	// 추격도 안되면 대기 
	bool bWait = Tick_CheckWait();
	CheckTrue(bWait);
}

void UCBTService_Melee::RadnomActionDelay()
{
	CurrentDelay = FMath::RandRange(ActionDelay.X, ActionDelay.Y);
}



bool UCBTService_Melee::Tick_CheckWait() const
{
	CheckNullResult(CachedBehavior, false);

	bool bCanMove = true;
	bCanMove = CachedBehavior->GetCanMove();
	if (bCanMove == false)
	{
		CachedBehavior->SetWaitMode();

		return true;
	}

	return false;
}

bool UCBTService_Melee::Tick_CheckPatrol(const ACharacter* InTarget)
{
	CheckNullResult(CachedBehavior, false); 
	
	if (InTarget == nullptr)
	{
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
	// 적이 감지되고 상대와의 거리가 일정하면 가드 올림.
	if (distance <= ActionRange)
	{
		CachedBehavior->SetGuardMode();
		//CachedState->SetGuardMode();

		return true;
	}


	CachedBehavior->SetWaitMode();

	return false;

}

bool UCBTService_Melee::Tick_CheckApproach(const ACharacter* InTarget)
{
	CheckNullResult(CachedBehavior, false);
	CheckNullResult(InTarget, false);


	// 적이 있고 공격할 수 있을 때 공격 범위 밖이면 공격하러 감
	float distance = CachedAI->GetDistanceTo(InTarget);
	if (CurrentDelay <= 0.0f && distance > ActionRange)
	{
		CachedBehavior->SetApproachMode();

		return true; 
	}

	return false;
}
