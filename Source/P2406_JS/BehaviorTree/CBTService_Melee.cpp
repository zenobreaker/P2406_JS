#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/IGuardable.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CHealthPointComponent.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "GameInstances/CBattleManager.h"
#include "GameInstances/CGameInstance.h" 

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

		Blackboard = SearchData.OwnerComp.GetBlackboardComponent();
	}
}
static int notifyCallCount = 0; // 호출 횟수 제한용
const int maxNotifyCalls = 5;  // 최대 호출 횟수

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CheckNull(CachedBehavior);
	CheckNull(CachedState);

	bool bCheck = false;
	bCheck |= (CachedBehavior->IsDeadMode());
	//	bCheck |= (CachedBehavior->IsDamageMode());
	bCheck |= (CachedState->IsDeadMode());
	bCheck |= (CachedState->IsDamagedMode());
	bCheck |= CachedBehavior->GetCanMove() == false;
	if (bCheck)
	{
		SetFocus(nullptr);
		CachedBehavior->SetNoneMode();

		return;
	}


	Calc_DelayTimer(DeltaSeconds);

	// 감지된 적 
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();

	if (target == nullptr)
	{
		// 적이 없으면 배틀매니저에게 호출
		Notify_Battle_FindBattle(&target);
		notifyCallCount--;
		if (target == nullptr)
		{
			Blackboard->SetValueAsBool("bInBattle", false);
			// 적이 없으면 그냥 순찰할래
			CachedBehavior->SetPatrolMode();
			// 비전투면 이 플래그를 해제 
			isFirstAttack = false;
			
			return;
		}
	}
	if (!!Blackboard)
		Blackboard->SetValueAsBool("bInBattle", true);

	// 적을 발견 했다면 결정한다. 
	// 1. 먼저 대기 하거나 적을 보면서 순찰한다.

	// 2. 공격 조건이 달성되면 공격하러 간다. 

	CheckTrue(CachedBehavior->IsActionMode());
	if (CurrentDelay > 0.0f)
		return;

	if (CachedState->IsIdleMode() == false)
		return;

	UCStateComponent* targetState = FHelpers::GetComponent<UCStateComponent>(target); 
	if (targetState && targetState->IsDownMode())
	{
		CachedBehavior->SetWaitMode();

		return; 
	}

	float distance = CachedAI->GetDistanceTo(target);
	if (distance <= ActionRange)
	{
		CachedBehavior->SetActionMode();
		if(isFirstAttack == false)
			isFirstAttack = true;
		RadnomActionDelay();

		return;
	}

	CachedBehavior->SetApproachMode();

	Notify_Battle_JoinBattle(CachedAI->GetGroupID(), CachedAI, target);
}

void UCBTService_Melee::RadnomActionDelay()
{
	CurrentDelay = FMath::RandRange(ActionDelay.X, ActionDelay.Y);
}



void UCBTService_Melee::SetFocus(ACharacter* InTarget) const
{
	CheckNull(CachedController);

	if (InTarget == nullptr)
	{
		CachedController->ClearFocus(EAIFocusPriority::Gameplay);

		return;
	}

	CachedController->SetFocus(InTarget);
}

void UCBTService_Melee::Notify_Battle_FindBattle(class ACharacter** OutTarget)
{
	CheckNull(Blackboard);
	CheckNull(CachedAI);


	if (notifyCallCount >= maxNotifyCalls)
	{
		FLog::Print("Notify_Battle_FindBattle exceeded max calls.", 2700);
		return;
	}
	notifyCallCount++;


	bool value = Blackboard->GetValueAsBool("bInBattle");
	if (value == false)
		return;

	// 배틀매니저한테 전투 끝났으니까 주위에 다른 친구 싸우고 있어? 물어봄
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	// 이전에 한 번 전투 돌입 상태였다면 
	if (!!instance && value)
	{
		UCBattleManager* battleManager = instance->BattleManager;

		if (!!battleManager)
		{
			ACharacter* target = battleManager->GetBattleAttackerOfTarget(CachedAI->GetGroupID(), CachedAI);
			*OutTarget = target;
			if (*OutTarget != nullptr)
				FLog::Print("Battle!!! ", 2700);

			return;
		}
	}
}

void UCBTService_Melee::Notify_Battle_JoinBattle(int32 InGroupID, class ACEnemy_AI* Initiator, class ACharacter* InTarget)
{
	CheckNull(Initiator);
	CheckNull(InTarget); 


	// 첫 전투 돌입 시에 호출
	// 체력이 없을 때 호출 
	// 공격 대상이 바꼈을 때? 
	// 대미지 입었을 때? 

	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance);
	UCBattleManager* battleManager = instance->BattleManager;

	CheckNull(battleManager);

	if (isFirstAttack)
	{
		battleManager->RequestBattleParticipation(InGroupID, Initiator, InTarget);
	
	}

}

void UCBTService_Melee::Calc_DelayTimer(float DeltaSeconds)
{
	// 공격 딜레이 타이머 
	if (CurrentDelay > 0.0f)
	{
		CurrentDelay -= DeltaSeconds;
		FLog::Print(CachedAI->GetName() + "Current Delay " + FString::SanitizeFloat(CurrentDelay), 1992 + CachedAI->GetAIID());
	}
}

