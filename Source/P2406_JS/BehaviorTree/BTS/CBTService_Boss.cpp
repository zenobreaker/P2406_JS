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

	// �ൿ �Ҵ� ���ǵ� üũ 
	bool bCheck = false;
	bCheck |= (CachedBehavior->IsDeadMode());
	bCheck |= Blackboard->GetValueAsBool("bCanAct") == false;
	bCheck |= CachedAI->GetCanAct() == false;

	// TODO: �����̻��� �ϴ� ����
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();
	bCheck &= target == nullptr;
	if (bCheck)
	{
		// ������.. �׸�.. 
		CachedBehavior->SetNoneMode();
		//SetFocus(nullptr);

		return;
	}

	// ������ ���� ���̸� ���� ����
	bool bPatternExecute = CachedPattern->IsExecutePattern();
	if (bPatternExecute == true)
		return;

	bool bDecidedPattern = CachedPattern->GetDecidePattern();
	if(bDecidedPattern == false)
	{	
		// ���� ���� �س��� 
		//CachedPattern->DecidePattern();
		
		// ���� �����ӿ��� ��Ÿ��� ����ϱ� ���ؼ� 
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
