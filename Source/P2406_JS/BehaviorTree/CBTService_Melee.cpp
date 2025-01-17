#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/IGuardable.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CHealthPointComponent.h"

UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Melee";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_Melee::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!CachedController)
	{
		CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());

		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
		CachedState = FHelpers::GetComponent<UCStateComponent>(CachedAI);
		CachedGuardable = Cast<IIGuardable>(CachedAI);
	}
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CheckTrue(CachedBehavior->IsDeadMode());
	CheckTrue(CachedBehavior->IsDamageMode());

	// 공격 딜레이 타이머 
	if (CurrentDelay > 0.0f)
	{
		CurrentDelay -= DeltaSeconds;
		FLog::Print(CachedAI->GetName() + "Current Delay " + FString::SanitizeFloat(CurrentDelay), 1992 + CachedAI->GetAIID());
	}

	// 행동 제약 상태라면 행동을 개시하지 않는다. 
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();

	// 내가 움직일 수 있는 상태인지 
	bool bCanMove = CachedBehavior->GetCanMove();
	if (bCanMove == false)
	{
		//TODO: 음 로직이 뭔가 맘에 안드는데 
		bool bWait = Tick_CheckWait();

		SetFocus(nullptr);

		CheckTrue(bWait);
	}

	// 여기에서 뻗는다 
	// target이 중간에 nullptr이 되서 그런가?
	SetFocus(target);

	// 타겟이 없으면 순찰
	bool bPatrol = Tick_CheckPatrol(target);
	CheckTrue(bPatrol);

	// 범위 내에 적이 있으면 공격
	bool bAttack = Tick_CheckAttack(target);
	CheckTrue(bAttack);

	// 적이 있고 공격할 수 있을 때 공격 범위 밖이면 공격하러 감
	bool bApproach = Tick_CheckApproach(target);
	CheckTrue(bApproach);


	// 적이 있고 위에 조건들이 통합되지 않으면 회피(배회)
	bool bAvoid = Tick_CheckAvoid(target);
	CheckTrue(bAvoid);


	// 추격도 안되면 대기 
	bool bWait = Tick_CheckWait();
	CheckTrue(bWait);

	//{
	//	FVector AIPosition = GetActorLocation();
	//	FVector TargetPosition = Target->GetActorLocation();
	//	float Distance = FVector::Dist(AIPosition, TargetPosition);

	//	FLog::Print("AI Position: " + AIPosition.ToString(), 1992);
	//	FLog::Print("Target Position: " + TargetPosition.ToString(), 1992);
	//	FLog::Print("Calculated Distance: " + FString::SanitizeFloat(Distance), 1992);
	//	FLog::Print("Max Distance: " + FString::FromInt(MaxDistance), 1992);
	//}
}

void UCBTService_Melee::RadnomActionDelay()
{
	CurrentDelay = FMath::RandRange(ActionDelay.X, ActionDelay.Y);
}



bool UCBTService_Melee::Tick_CheckWait() const
{
	CheckNullResult(CachedBehavior, false);

	if (CachedBehavior->IsActionMode() || CachedBehavior->IsApproachMode())
		return false;

	CachedBehavior->SetWaitMode();

	return true;

}

bool UCBTService_Melee::Tick_CheckAvoid(const ACharacter* InTarget) const
{
	CheckNullResult(CachedBehavior, false);
	CheckFalseResult(InTarget != nullptr, false);


	if (CachedBehavior->IsActionMode() || CachedBehavior->IsApproachMode())
		return false;

	CachedBehavior->SetAvoidMode();


	return true;
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

	// 내가 공격 상태면 공격을 다했는지 알아야 한다. 
	if (CachedBehavior->IsActionMode())
	{

	}


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
//
bool UCBTService_Melee::Tick_CheckApproach(const ACharacter* InTarget)
{
	CheckNullResult(CachedBehavior, false);
	CheckNullResult(InTarget, false);


	// 적이 있고 공격할 수 있을 때 공격 범위 밖이면 공격하러 감
	float distance = CachedAI->GetDistanceTo(InTarget);
	if (CurrentDelay <= 0.0f && distance > ActionRange)
	{
		if (CachedState->IsIdleMode() == false)
		{
			return false;
		}

		CachedBehavior->SetApproachMode();

		return true;
	}

	return false;
}

void UCBTService_Melee::SetFocus(ACharacter* InTarget) const
{
	CheckNull(CachedController);

	if (InTarget == nullptr)
	{
		CachedController->ClearFocus(EAIFocusPriority::Gameplay);

		return;
	}

	CachedController->SetFocus(InTarget);
}



//bool UCBTService_Melee::Tick_CheckGuard(const ACharacter* InTarget) const
//{
//	CheckNullResult(CachedBehavior, false);
//	CheckNullResult(CachedState, false);
//	CheckNullResult(CachedAI, false);
//	CheckNullResult(InTarget, false);
//
//	bool bGuardale = false;
//	if (CachedGuardable)
//	{
//		bGuardale = true;
//	}
//
//	if (bGuardale == false)
//	{
//		return false;
//	}
//	
//
//	float distance = CachedAI->GetDistanceTo(InTarget);
//	// 적이 감지되고 상대와의 거리가 일정하면 가드 올림.
//	if (distance <= ActionRange)
//	{
//		CachedBehavior->SetGuardMode();
//		//CachedState->SetGuardMode();
//
//		return true;
//	}
//
//	return false;
//}
