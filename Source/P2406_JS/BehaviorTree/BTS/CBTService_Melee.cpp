#include "CBTService_Melee.h"
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
	bCheck |= Blackboard->GetValueAsBool("bCanAct") == false;
	bCheck |= CachedAI->GetCanAct() == false;

	if (bCheck)
	{
		CachedBehavior->SetNoneMode();
		SetFocus(nullptr);

		return;
	}

	Calc_DelayTimer(DeltaSeconds);

	// 감지된 적 
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();

	// 적이 없으면 순찰 
	if (target == nullptr)
	{
		// 적이 없으면 배틀매니저에게 호출
		Notify_Battle_FindBattle(&target);
		notifyCallCount--;
		if (target == nullptr)
		{
			// 비전투면 이 플래그를 해제 
			Blackboard->SetValueAsBool("bInBattle", false);
			Blackboard->SetValueAsBool("bFirstAttack", false);
			Blackboard->SetValueAsBool("bFirstDamage", false);
			
			// 적이 없으면 그냥 순찰할래
			CachedBehavior->SetPatrolMode();
			SetFocus(nullptr);
			return;
		}
	}


	//TODO: 이 내용은 조금 복잡해지는 내용이므로 어드밴스드 서비스나 다른 서비스에서 하는 것도
	UCConditionComponent* targetCondition = FHelpers::GetComponent<UCConditionComponent>(target);
	if ((targetCondition && targetCondition->GetDownCondition()))
	{
		CachedBehavior->SetWaitMode();

		return;
	}


	float distance = CachedAI->GetDistanceTo(target);
	if (CurrentDelay > 0.0f)
	{

		if (distance > WaitRange)
			CachedBehavior->SetApproachMode();
		else if(CachedBehavior->IsActionMode() == false)
			CachedBehavior->SetWaitMode();

		return;
	}

	// 블랙보드에서 값 확인 todo 이거 이름 바꾼다 bSelectedAttacker
	bool bContainAttacker = Blackboard->GetValueAsBool("IsContainedInBattle");

	bool isAttackable = false;
	// 블랙보드에 값이 없거나 공격자 명단에 없는 경우 업데이트
	if (bContainAttacker == false)
	{
		isAttackable = IsTargetAttackable(target);
		if (isAttackable == false)
		{
			RadnomActionDelay();

			return;
		}

		// 배틀 매니저에 등록 후 블랙보드 갱신
		Notify_Battle_SetJoinBattle(target, CachedAI);
		Blackboard->SetValueAsBool("IsContainedInBattle", true);
	}


	// 공격 기능
	if (distance <= ActionRange && CurrentDelay <= 0.0f)
	{
		bool bFirstAttack = Blackboard->GetValueAsBool("bFirstAttack");
		if (bFirstAttack == false)
		{
			Blackboard->SetValueAsBool("bFirstAttack", true);
		}

		CachedBehavior->SetActionMode();

		// 공격 완료되었으니 
		// 그럼 배틀 매니저에게 자신을 탈락 시켜 달라고 전달 
		Notify_Battle_RemoveBattle(target, CachedAI);

		RadnomActionDelay();

		return;
	}
	else
	{
		if (CachedBehavior->IsActionMode() || CachedBehavior->IsApproachMode())
			return;

		CachedBehavior->SetApproachMode();
	}

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

bool UCBTService_Melee::Notify_Battle_IsContainAttacker(ACharacter* InTarget)
{
	CheckNullResult(InTarget, false);

	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNullResult(instance, false);
	UCBattleManager* battleManager = instance->BattleManager;
	CheckNullResult(battleManager, false);

	return battleManager->IsContainFromAttackers(InTarget, CachedAI);
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

//void UCBTService_Melee::Notify_Battle_JoinBattle(int32 InGroupID, class ACEnemy_AI* Initiator, class ACharacter* InTarget)
//{
//	CheckNull(Initiator);
//	CheckNull(InTarget);
//
//	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
//	CheckNull(instance);
//	UCBattleManager* battleManager = instance->BattleManager;
//	CheckNull(battleManager);
//
//	// 체력이 없을 때 호출 
//	// 공격 대상이 바꼈을 때? 
//
//
//	bool bCheck = false; 
//	bCheck |= isFirstAttack;
//	bCheck |= isFirstDamage;
//	// 대미지 입었을 때? 
//	// 첫 전투 돌입 시에 호출
//	if (bCheck)
//	{
//		
//		battleManager->RequestBattleParticipation(InGroupID, Initiator, InTarget);
//		return;
//	}
//}

void UCBTService_Melee::Notify_Battle_RemoveBattle(ACharacter* InTarget, ACEnemy_AI* Initiator)
{
	CheckNull(Initiator);
	CheckNull(InTarget);


	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance);

	UCBattleManager* battleManager = instance->BattleManager;
	CheckNull(battleManager);

	battleManager->UnregistBattle(InTarget, Initiator);

	if (!!Blackboard)
	{
		Blackboard->SetValueAsBool("bInBattle", false);
		Blackboard->SetValueAsBool("IsContainedInBattle", false);
	}

}

void UCBTService_Melee::Notify_Battle_SetJoinBattle(ACharacter* InTarget, ACEnemy_AI* Initiator)
{
	CheckNull(Initiator);
	CheckNull(InTarget);

	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance);

	UCBattleManager* battleManager = instance->BattleManager;
	CheckNull(battleManager);

	battleManager->RegistBattle(InTarget, Initiator);

	if (!!Blackboard)
		Blackboard->SetValueAsBool("bInBattle", true);
}



bool UCBTService_Melee::IsTargetAttackable(ACharacter* InTarget)
{
	CheckNullResult(InTarget, false);

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

