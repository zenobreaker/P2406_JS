#include "BehaviorTree/BTS/CBTService_FocusOnTarget.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CAIBehaviorComponent.h"

UCBTService_FocusOnTarget::UCBTService_FocusOnTarget()
{
	NodeName = "FoucsOnTarget";
	Interval = 0.1f;
}

void UCBTService_FocusOnTarget::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!CachedController)
	{
		CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());
		CheckNull(CachedAI);

		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
	}
}

void UCBTService_FocusOnTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (!!CachedBehavior && !!CachedController)
	{
		// Å¸°Ù °Ë»ç 
		ACharacter* target = nullptr;
		target = CachedBehavior->GetTarget();
		CachedController->SetFocus(target);
	}
}

