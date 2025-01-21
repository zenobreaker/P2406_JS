#include "BehaviorTree/CBTService_Melee.h"
#include "Global.h"

#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/IGuardable.h"

#include "Components/CStateComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CConditionComponent.h"

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

	// 행동 불능 조건들 체크 
	bool bCheck = false;
	bCheck |= (CachedBehavior->IsDeadMode());
	//	bCheck |= (CachedBehavior->IsDamageMode());
	bCheck |= (CachedState->IsDeadMode());
	bCheck |= (CachedState->IsDamagedMode());
	bCheck |= CachedBehavior->GetCanMove() == false;
	//bCheck |= CachedState->IsIdleMode() == false;
	
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

	ChangeTarget(target);

	// 적이 없으면 순찰 
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
		//	isFirstAttack = false;

			return;
		}
	}
	if (!!Blackboard)
		Blackboard->SetValueAsBool("bInBattle", true);




	// 적을 발견 했다면 결정한다. 
	// 1. 먼저 대기 하거나 적을 보면서 순찰한다.

	// 2. 공격 조건이 달성되면 공격하러 간다. 

	//CheckTrue(CachedBehavior->IsActionMode());
	//TODO: 이 내용은 조금 복잡해지는 내용이므로 어드밴스드 서비스나 다른 서비스에서 하는 것도
	UCConditionComponent* targetCondition = FHelpers::GetComponent<UCConditionComponent>(target);
	if ((targetCondition && targetCondition->GetDownCondition()) )
	{
		CachedBehavior->SetWaitMode();

		return;
	}

	if (CurrentDelay > 0.0f)
		return; 

	bool isAttackable = IsTargetAttackable(target);
	if (isAttackable == false)
		return; 

	float distance = CachedAI->GetDistanceTo(target);
	if (distance <= ActionRange)
	{
		// ㅈ같은 Check 이상한거 만들어서 개고생한다. 
		CachedBehavior->SetActionMode();
		
		FLog::Print("Enemy Attack");

		if (isFirstAttack == false)
		{
			isFirstAttack = true;
			Notify_Battle_JoinBattle(CachedAI->GetGroupID(), CachedAI, target);
		}

		RadnomActionDelay();

		return;
	}
	//else if(IsTargetAttackable(target) == false)
	//{
	//	// 쿨타임이 돌고 있을 땐 대기 상태로 
	//	if (CachedBehavior->IsWaitMode() == false)
	//		CachedBehavior->SetWaitMode();

	//	return;
	//}

	if (CachedBehavior->IsApproachMode())
		return;

	CachedBehavior->SetApproachMode();
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
				FLog::Print("Battle!!! " + (*OutTarget)->GetName(), 2700);

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

void UCBTService_Melee::ChangeTarget(ACharacter* InTarget)
{
	if (PrevTarget != InTarget)
	{
		// 새로운 타겟은 배틀매니저한테 해당 타겟으로 싸운다고 등록한다. 
	// 배틀매니저한테 전투 끝났으니까 주위에 다른 친구 싸우고 있어? 물어봄
		UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

		CheckNull(instance);

		UCBattleManager* battleManager = instance->BattleManager;
		CheckNull(battleManager);

		// 이전에 등록한 대상으로는 지워 
		battleManager->UnregisterAttacker(PrevTarget, CachedAI);

		// 새로운 타겟으로 싸우자 신청 ㄱㄱ
		battleManager->RegisterAttacker(InTarget, CachedAI);

		PrevTarget = InTarget;
	}
}

bool UCBTService_Melee::IsTargetAttackable(ACharacter* InTarget)
{
	// 배틀매니저한테 전투 끝났으니까 주위에 다른 친구 싸우고 있어? 물어봄
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	CheckNullResult(instance, true);

	UCBattleManager* battleManager = instance->BattleManager;
	CheckNullResult(battleManager, true);

	return battleManager->IsAttackableToTarget(InTarget, CachedAI);
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

