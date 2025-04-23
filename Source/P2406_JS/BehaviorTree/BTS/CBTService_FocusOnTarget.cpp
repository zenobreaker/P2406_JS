#include "BehaviorTree/BTS/CBTService_FocusOnTarget.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CConditionComponent.h"

UCBTService_FocusOnTarget::UCBTService_FocusOnTarget()
{
	NodeName = "FoucsOnTarget";
	//Interval = 0.1f;
}

void UCBTService_FocusOnTarget::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!CachedController)
	{
		CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());
		CheckNull(CachedAI);
		
		Condition = FHelpers::GetComponent<UCConditionComponent>(CachedAI);
		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
	}
}

void UCBTService_FocusOnTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (CachedBehavior == nullptr || CachedController == nullptr)
	{
		return;
	}

	if (Condition != nullptr && (Condition->GetDownCondition() || Condition->GetAirborneCondition()))
	{
		CachedController->ClearFocus(EAIFocusPriority::Gameplay);
		return; 
	}

	// Å¸°Ù °Ë»ç 
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();
	if (target != nullptr)
	{
		UCAIBehaviorComponent* otherHavior = FHelpers::GetComponent<UCAIBehaviorComponent>(target);
		if (!!otherHavior)
		{
			if (otherHavior->IsDeadMode())
			{
				CachedController->ClearFocus(EAIFocusPriority::Gameplay);

				return;
			}
		}

		CachedController->SetFocus(target);
	}

}

void UCBTService_FocusOnTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (!!CachedBehavior && !!CachedController)
	{
		ACharacter* target = nullptr;
		target = CachedBehavior->GetTarget();
		if (target != nullptr)
		{
			CachedController->SetFocus(target);
		}
	}
}

void UCBTService_FocusOnTarget::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	if (!!CachedBehavior && !!CachedController)
	{
		CachedController->ClearFocus(EAIFocusPriority::Gameplay);
	}
}

