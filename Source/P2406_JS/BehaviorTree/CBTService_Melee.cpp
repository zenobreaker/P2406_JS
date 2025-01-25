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
static int notifyCallCount = 0; // ȣ�� Ƚ�� ���ѿ�
const int maxNotifyCalls = 5;  // �ִ� ȣ�� Ƚ��

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CheckNull(CachedBehavior);
	CheckNull(CachedState);

	// �ൿ �Ҵ� ���ǵ� üũ 
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

	// ������ �� 
	ACharacter* target = nullptr;
	target = CachedBehavior->GetTarget();

	// ���� ������ ���� 
	if (target == nullptr)
	{
		// ���� ������ ��Ʋ�Ŵ������� ȣ��
		Notify_Battle_FindBattle(&target);
		notifyCallCount--;
		if (target == nullptr)
		{
			Blackboard->SetValueAsBool("bInBattle", false);
			// ���� ������ �׳� �����ҷ�
			CachedBehavior->SetPatrolMode();
			// �������� �� �÷��׸� ���� 
		//	isFirstAttack = false;

			return;
		}
	}

	//TODO: �� ������ ���� ���������� �����̹Ƿ� ���꽺�� ���񽺳� �ٸ� ���񽺿��� �ϴ� �͵�
	UCConditionComponent* targetCondition = FHelpers::GetComponent<UCConditionComponent>(target);
	if ((targetCondition && targetCondition->GetDownCondition()))
	{
		CachedBehavior->SetNoneMode();

		return;
	}


	if (CurrentDelay > 0.0f)
	{
		if(CachedBehavior->IsActionMode() == false)
			CachedBehavior->SetWaitMode();

		return;
	}

	// ���� ������ ��ܿ� �ִ��� Ȯ��
	bool bContainAttacker = Notify_Battle_IsContainAttakcer(target);
	// ��ܿ� �����Ƿ� �߰� 
	if (bContainAttacker == false)
	{
		// ���� ���ݰ����� �������� ����� 
		bool isAttackable = true;
		isAttackable &= IsTargetAttackable(target);
		if (isAttackable == false)
		{
			RadnomActionDelay();

			return;
		}

		// ��Ʋ�Ŵ������� ���
		Notify_Battle_SetJoinBattle(target, CachedAI);
	}

	// ���� ���
	float distance = CachedAI->GetDistanceTo(target);
	if (distance <= ActionRange)
	{
		CachedBehavior->SetActionMode();

		FLog::Print("Enemy Attack");

		if (isFirstAttack == false)
		{
			isFirstAttack = true;
			Notify_Battle_JoinBattle(CachedAI->GetGroupID(), CachedAI, target);
		}

		// ���� �Ϸ�Ǿ����� 
		// �׷� ��Ʋ �Ŵ������� �ڽ��� Ż�� ���� �޶�� ���� 
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

bool UCBTService_Melee::Notify_Battle_IsContainAttakcer(ACharacter* InTarget)
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

	// ��Ʋ�Ŵ������� ���� �������ϱ� ������ �ٸ� ģ�� �ο�� �־�? ���
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	// ������ �� �� ���� ���� ���¿��ٸ� 
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


	// ù ���� ���� �ÿ� ȣ��
	// ü���� ���� �� ȣ�� 
	// ���� ����� �ٲ��� ��? 
	// ����� �Ծ��� ��? 

	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance);
	UCBattleManager* battleManager = instance->BattleManager;
	CheckNull(battleManager);

	if (isFirstAttack)
	{
		battleManager->RequestBattleParticipation(InGroupID, Initiator, InTarget);

	}

}

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
		Blackboard->SetValueAsBool("bInBattle", false);

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

	// ��Ʋ�Ŵ������� ���� �������ϱ� ������ �ٸ� ģ�� �ο�� �־�? ���
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	CheckNullResult(instance, true);

	UCBattleManager* battleManager = instance->BattleManager;
	CheckNullResult(battleManager, true);

	return battleManager->IsAttackableToTarget(InTarget, CachedAI);
}

void UCBTService_Melee::Calc_DelayTimer(float DeltaSeconds)
{
	// ���� ������ Ÿ�̸� 
	if (CurrentDelay > 0.0f)
	{
		CurrentDelay -= DeltaSeconds;
		FLog::Print(CachedAI->GetName() + "Current Delay " + FString::SanitizeFloat(CurrentDelay), 1992 + CachedAI->GetAIID());
	}
}

