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


	// ���� ������ Ÿ�̸� 
	if (CurrentDelay > 0.0f)
	{
		CurrentDelay -= DeltaSeconds;
		FLog::Print("Current Delay " + FString::SanitizeFloat (CurrentDelay), 1992);
	}

	// �ൿ ���� ���¶�� �ൿ�� �������� �ʴ´�. 
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();

	
	// ���� ������ �� �ִ� �������� 
	bool bCanMove = CachedBehavior->GetCanMove();
	if (bCanMove == false)
	{
		//TODO: �� ������ ���� ���� �ȵ�µ� 
		bool bWait = Tick_CheckWait();

		SetFocus(nullptr);

		CheckTrue(bWait);
	}

	SetFocus(target);

	// Ÿ���� ������ ����
	bool bPatrol = Tick_CheckPatrol(target);
	CheckTrue(bPatrol);

	// ���� ���� ���� ������ ����
	bool bAttack = Tick_CheckAttack(target);
	CheckTrue(bAttack);

	// ���� �ְ� ������ �� ���� �� ���� ���� ���̸� �����Ϸ� ��
	bool bApproach = Tick_CheckApproach(target);
	CheckTrue(bApproach);


	// ���� �ְ� ���� ���ǵ��� ���յ��� ������ ȸ��(��ȸ)
	bool bAvoid = Tick_CheckAvoid(target); 
	CheckTrue(bAvoid);


	// �߰ݵ� �ȵǸ� ��� 
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

	bool bCanMove = true;
	bCanMove = CachedBehavior->GetCanMove();
	if (bCanMove == false)
	{
		CachedBehavior->SetWaitMode();

		return true;
	}

	return false;
}

bool UCBTService_Melee::Tick_CheckAvoid(const ACharacter* InTarget) const
{
	CheckNullResult(CachedBehavior, false);
	CheckFalseResult(InTarget != nullptr, false); 
	

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


	// ���� �ְ� ������ �� ���� �� ���� ���� ���̸� �����Ϸ� ��
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

void UCBTService_Melee::SetFocus(ACharacter* InTarget)
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
//	// ���� �����ǰ� ������ �Ÿ��� �����ϸ� ���� �ø�.
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
