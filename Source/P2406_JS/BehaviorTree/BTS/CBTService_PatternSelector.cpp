#include "BehaviorTree/BTS/CBTService_PatternSelector.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CBoss_AI.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CPatternComponent.h"


UCBTService_PatternSelector::UCBTService_PatternSelector()
{
	NodeName = "PhaseSelector";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_PatternSelector::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!CachedController)
	{
		Blackboard = SearchData.OwnerComp.GetBlackboardComponent();

		CachedAI = Cast<ACBoss_AI>(CachedController->GetPawn());

		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
		CachedPattern = FHelpers::GetComponent<UCPatternComponent>(CachedAI);
	}
}

void UCBTService_PatternSelector::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


}
