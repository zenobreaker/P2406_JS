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


	// ���� ������ Ÿ�̸� 
	if (CurrentDelay > 0.0f)
		CurrentDelay -= DeltaSeconds;

	// �ൿ ���� ���¶�� �ൿ�� �������� �ʴ´�. 
	//CheckTrue(Tick_CheckWait());
	/*ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();
	bool bCheck = Tick_CheckGuard(target);
	if (bCheck)
	{
		return; 
	}*/
	

	CachedBehavior->SetWaitMode();
	//bool bCanMove = true;
	//bCanMove = CachedBehavior->GetCanMove();
	////if (bCanMove == false)
	//{
	//	CachedBehavior->SetWaitMode();
	//}


	// Ÿ���� ������ ����
	//CheckTrue(Tick_CheckPatrol(&target));
	// ���尡 �����ϸ� ���� �ϱ�

	// ���� ���� ���� ������ ����
//	CheckTrue(Tick_CheckAttack(target));

	// �� �ܿ� �߰�
	//CheckTrue(Tick_CheckApproach(target));

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

bool UCBTService_Melee::Tick_CheckPatrol(ACharacter** OutTarget) const
{
	CheckNullResult(CachedBehavior, false);

	*OutTarget = CachedBehavior->GetTarget();
	if (*OutTarget == nullptr)
	{
		CachedBehavior->SetPatrolMode();

		return true;
	}

	return false;
}

bool UCBTService_Melee::Tick_CheckAttack(const ACharacter* InTarget)
{
	if (CachedBehavior == nullptr)
		return false; 

	if (CachedState == nullptr)
		return false;
	if (CachedAI == nullptr)
		return false;

	if (InTarget == nullptr)
		return false;

	/*CheckNullResult(CachedBehavior, false);
	CheckNullResult(CachedState, false);
	CheckNullResult(CachedAI, false);
	CheckNullResult(InTarget, false);*/

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
	// ���� �����ǰ� ������ �Ÿ��� �����ϸ� ���� �ø�.
	if (distance <= ActionRange)
	{
		CachedBehavior->SetGuardMode();
		//CachedState->SetGuardMode();

		return true;
	}


	CachedBehavior->SetWaitMode();

	return false;

}

bool UCBTService_Melee::Tick_CheckApproach(const ACharacter* InTarget) const
{
	CheckNullResult(CachedBehavior, false);
	CheckNullResult(InTarget, false);

	CachedBehavior->SetApproachMode();

	return true;
}
