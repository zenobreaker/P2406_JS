#include "BehaviorTree/BTS/CBTService_Guard.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/IGuardable.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CGuardComponent.h"
#include "Components/CConditionComponent.h"

#include "BehaviorTree/BlackboardComponent.h"


UCBTService_Guard::UCBTService_Guard()
{
	NodeName = "Guard";
}

void UCBTService_Guard::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	CheckNull(CachedController);

	CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());
	CheckNull(CachedAI);

	CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
	CachedState = FHelpers::GetComponent<UCStateComponent>(CachedAI);

	GuardComp = FHelpers::GetComponent<UCGuardComponent>(CachedAI);
	Condition = FHelpers::GetComponent<UCConditionComponent>(CachedAI); 

	Blackboard = SearchData.OwnerComp.GetBlackboardComponent();
}

void UCBTService_Guard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	CheckFalse(CachedBehavior->GetCanMove());

	auto* target = CachedBehavior->GetTarget();
	if (target == nullptr)
	{
		// Ÿ���� ���ٰ�? �׷� ���и� ������ ����. 
		Blackboard->SetValueAsBool("bGuarding", false);

		return;
	}

	// �� ���񽺴� ���带 �� �� �ְ� �Ѵ�. 
	CalcParryTimer();

	OnGuardState();
}

void UCBTService_Guard::CalcParryTimer()
{
	CheckFalse(CurrentParry <= 0.0f);

	float minValue = MaxParryTimer - ParryRandomDiff;
	float maxValue = MaxParryTimer + ParryRandomDiff;

	CurrentParry = FMath::RandRange(minValue, maxValue);
}

void UCBTService_Guard::OnCounterState()
{
	CheckNull(GuardComp);

	GuardComp->StartCounterGuard();
}

void UCBTService_Guard::OnGuardState()
{
	CheckNull(CachedBehavior);

	bool bCheck = true; 
	bCheck &= GuardComp->GetCanGuard() == true;
	bCheck &= GuardComp->GetGuarding() == false;
	Blackboard->SetValueAsBool("bGuarding", bCheck);
}
