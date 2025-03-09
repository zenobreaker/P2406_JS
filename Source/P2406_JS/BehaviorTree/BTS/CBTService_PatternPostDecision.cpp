#include "BehaviorTree/BTS/CBTService_PatternPostDecision.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"


#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CPatternComponent.h"

UCBTService_PatternPostDecision::UCBTService_PatternPostDecision()
{
	NodeName = "Pattern Post Decision";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
	bNotifyBecomeRelevant = true; 
	//bNotifyCeaseRelevant = true;
}

void UCBTService_PatternPostDecision::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!CachedController)
	{
		Blackboard = SearchData.OwnerComp.GetBlackboardComponent();

		CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());

		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
		CachedPattern = FHelpers::GetComponent<UCPatternComponent>(CachedAI);
	}
}


void UCBTService_PatternPostDecision::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	CheckNull(CachedBehavior);
	CheckNull(CachedAI);

	if (CachedBehavior->GetPatternExecute() == false && CachedBehavior->IsWaitMode())
	{
		if (FMath::RandRange(0, 100) > StafeRate)
			CachedBehavior->SetStrafe(true);
		else
			CachedBehavior->SetStrafe(false);
	}
}

