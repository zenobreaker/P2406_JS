#include "BehaviorTree/BTS/CBTService_PhaseService.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CBoss_AI.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CPatternComponent.h"


UCBTService_PhaseService::UCBTService_PhaseService()
{
	NodeName = "PhaseService";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_PhaseService::OnSearchStart(FBehaviorTreeSearchData& SearchData)
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

void UCBTService_PhaseService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CheckNull(CachedAI);
	CheckNull(CachedBehavior);

	int32 currentPhase = CachedBehavior->GetCurrentPhase();
	int32 lastPhase = CachedBehavior->GetLastPhase();

	if (currentPhase > lastPhase)
	{
		//CachedBehavior->SetLastPhase(currentPhase);
		
		return; 
	}

	CachedAI->CheckPhaseTransition();

}
