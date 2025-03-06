#include "BehaviorTree/BTS/CBTService_Boss.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CPatternComponent.h"

#include "GameInstances/CBattleManager.h"
#include "GameInstances/CGameInstance.h" 

UCBTService_Boss::UCBTService_Boss()
{
	NodeName = "Boss";

	Interval = 0.1f; 
	RandomDeviation = 0.0f;
}

void UCBTService_Boss::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!CachedController)
	{
		Blackboard = SearchData.OwnerComp.GetBlackboardComponent();

		CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());
		CachedGuardable = Cast<IIGuardable>(CachedAI);

		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
		CachedState = FHelpers::GetComponent<UCStateComponent>(CachedAI);
		CachedPattern = FHelpers::GetComponent<UCPatternComponent>(CachedAI);
	}
}

void UCBTService_Boss::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	CheckNull(CachedBehavior);
	CheckNull(CachedState);
	CheckNull(CachedPattern);

	// 행동 불능 조건들 체크 
	bool bCheck = false;
	bCheck |= (CachedBehavior->IsDeadMode());
	bCheck |= Blackboard->GetValueAsBool("bCanAct") == false;
	bCheck |= CachedAI->GetCanAct() == false;
	
	if (bCheck)
	{
		CachedBehavior->SetNoneMode();
		//SetFocus(nullptr);

		return;
	}
	ACharacter* target = nullptr;	
	target = CachedBehavior->GetTarget();
	CheckNull(target);

	bool bDecidedPattern = CachedPattern->GetDecidePattern();
	bool bPatternExecute = CachedPattern->IsExecutePattern();

	bool bAboutPatternCheck= true; 
	bAboutPatternCheck &= bDecidedPattern == false;
	bAboutPatternCheck &= bPatternExecute == false;

	// 패턴이 수행 중이면 실행 안함
	if (bPatternExecute == true)
		return;

	// 패턴 결정 해놓음 
	CachedPattern->DecidePattern();

	//TODO: 이 내용은 조금 복잡해지는 내용이므로 어드밴스드 서비스나 다른 서비스에서 하는 것도
	UCConditionComponent* targetCondition = FHelpers::GetComponent<UCConditionComponent>(target);
	if ((targetCondition && targetCondition->GetDownCondition()) 
	|| bAboutPatternCheck)
	{
		CachedBehavior->SetWaitMode();
		
		return;
	}

	
	ActionRange = CachedBehavior->GetActionRange();
	float distance = CachedAI->GetDistanceTo(target);
	if (distance <= ActionRange)
	{
		CachedBehavior->SetActionMode();

		return; 
	}
	
	CachedBehavior->SetApproachMode();
}
