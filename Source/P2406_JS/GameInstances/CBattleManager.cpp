#include "GameInstances/CBattleManager.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

int32 UCBattleManager::MaxTokenValue = 1000;

UCBattleManager::UCBattleManager()
{

}

// 타겟을 비롯하여 그룹을 생성
void UCBattleManager::RegistGroup(int32 InGroupID, ACEnemy_AI* InMember)
{
	if (!GroupAITable.Contains(InGroupID))
		GroupAITable.Add(InGroupID, TArray <TWeakObjectPtr<ACEnemy_AI>>());

	TArray<TWeakObjectPtr<ACEnemy_AI>>& ais = GroupAITable[InGroupID];
	if (!ais.Contains(InMember))
	{
		ais.Add(InMember);
	}
}

void UCBattleManager::UnregistGroup(int32 InGroupID, ACEnemy_AI* InMember)
{
	if (!this || !IsValid(this))
		return;

	if (GroupAITable.Contains(InGroupID))
	{
		TArray<TWeakObjectPtr<ACEnemy_AI>>& ais = GroupAITable[InGroupID];
		ais.Remove(InMember);

		if (ais.Num() == 0)
			GroupAITable.Remove(InGroupID);
	}
}

ACharacter* UCBattleManager::GetBattleAttackerOfTarget(int32 InGroupID, ACEnemy_AI* InCaller)
{
	CheckNullResult(InCaller, nullptr);
	CheckFalseResult(GroupAITable.Contains(InGroupID), nullptr);
		
	ACharacter* bestTarget = nullptr;
	float closestDistance = FLT_MAX;

	// 가장 첫 번째 놈 반환
	for (const TWeakObjectPtr<ACEnemy_AI>& ai : GroupAITable[InGroupID])
	{
		if (ai == nullptr)
			continue;

		if (ai == InCaller)
			continue;

		UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent, ACEnemy_AI>(ai);
		if (behavior == nullptr || behavior->GetTarget() == nullptr)
			continue;

		float distance = FVector::Dist(ai->GetActorLocation(), behavior->GetTarget()->GetActorLocation());
		if (distance < closestDistance)
		{
			closestDistance = distance;
			bestTarget = behavior->GetTarget();
		}
	}

	return bestTarget;
}

void UCBattleManager::RequestBattleParticipation(int32 InGroupID, ACEnemy_AI* InInitiator, ACharacter* InTarget)
{
	CheckNull(InInitiator);
	CheckNull(InTarget);

	if (!GroupAITable.Contains(InGroupID))
		return;

	for (const TWeakObjectPtr<ACEnemy_AI>& ai : GroupAITable[InGroupID])
	{
		if (ai == InInitiator)
			continue;

		if (ai == nullptr)
			continue;

		UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent, ACEnemy_AI>(ai);
		if (behavior == nullptr)
			continue;
		auto* blackboard = behavior->GetBlackboard();
		if (blackboard == nullptr)
			continue;

		bool bCheck = blackboard->GetValueAsBool("bInBattle");
		if (bCheck == true)
			continue;

		auto* prevTarget = behavior->GetTarget();
		if (prevTarget != nullptr)
			continue;

		behavior->SetTarget(InTarget);
		FLog::Log(ai->GetName() + " Participanet! ");
	}
}

// 해당 타겟의 그룹의 등록
void UCBattleManager::RegistBattle(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	CheckNull(InTarget);
	CheckNull(InAttacker);

	if (IsValid(this) == false)
		return;

	if (TargetToAttackers.Contains(InTarget) == false)
	{
		// 타겟에게 죽으면 해제하도록이벤트 추가 
		ACBaseCharacter* character = Cast<ACBaseCharacter>(InTarget);
		if (character != nullptr)
			REGISTER_EVENT_WITH_REPLACE(character, OnCharacterDead_One, this, UCBattleManager::UnregistTarget);

		SetTokenTarget(InTarget);

		TargetToAttackers.Add(InTarget, TArray<TWeakObjectPtr<ACEnemy_AI>>());
	}

	TArray<TWeakObjectPtr<ACEnemy_AI>>& attackers = TargetToAttackers[InTarget];
	if (!attackers.Contains(InAttacker))
	{
		// 대상이 공격자인 경우 - 공격자가 죽으면 해제하도록 
		REGISTER_EVENT_WITH_REPLACE(InAttacker, OnCharacterDead_One, this, UCBattleManager::UnregistAttacker);
		// 그룹에 추가 될 때 
		SetTokenAttacker(InAttacker);

		attackers.Add(InAttacker);
	}
}

void UCBattleManager::UnregistBattle(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	UnregistAttacker(InTarget, InAttacker);
}

// 공격자 등록 제거 
void UCBattleManager::UnregistAttacker(ACharacter* InAttacker)
{
	CheckNull(InAttacker);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(InAttacker);
	CheckNull(ai);

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent, ACEnemy_AI>(ai);
	CheckNull(behavior);

	AActor* target = behavior->GetTarget();
	CheckNull(target);

	UnregistAttacker(target, ai);
}

void UCBattleManager::UnregistAttacker(AActor* InaTarget, ACEnemy_AI* InAttacker)
{
	CheckNull(InAttacker);
	CheckNull(InaTarget);

	if (TargetToAttackers.Contains(InaTarget) == false)
		return;

	TargetToAttackers[InaTarget].Remove(InAttacker);

	if (TargetToAttackers[InaTarget].Num() <= 0)
		TargetToAttackers.Remove(InaTarget);

	UnregistGroup(InAttacker->GetGroupID(), InAttacker);

	FLog::Log(InAttacker->GetName() + " Unregist Attacker ");
}

// 타겟 등록 제거 
void UCBattleManager::UnregistTarget(ACharacter* InInstigator)
{
	if (InInstigator->IsPendingKill())
		return; 

	CheckNull(InInstigator);

	// 이 대상이 등록되어 있지 않다면?
	if (!TargetToAttackers.Contains(InInstigator))
	{
		return;
	}


	TargetToTokenCount.Remove(InInstigator);
	TargetToAttackers.Remove(InInstigator);
}


void UCBattleManager::SetTokenAttacker(ACEnemy_AI* InAttacker)
{
	CheckNull(InAttacker);
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent, ACEnemy_AI>(InAttacker);
	CheckNull(behavior);

	int32 tokenValue = FMath::RandRange(0, MaxTokenValue);
	behavior->SetMyToken(tokenValue);
}

void UCBattleManager::SetTokenTarget(AActor* InTarget)
{
	CheckNull(InTarget);
	CheckTrue(TargetToTokenCount.Contains(InTarget));

	// 추가 된 적이 없을 때만 토큰 설정 
	// 최대 값은 오지 않게 절반만 해봄
	int32 token = FMath::RandRange(0, MaxTokenValue / 2);

	TargetToTokenCount.Add(InTarget, token);
}

bool UCBattleManager::IsContainFromAttackers(AActor* InTarget, class ACEnemy_AI* InAttacker) const
{
	CheckNullResult(InTarget, false);
	CheckNullResult(InAttacker, false);

	if (!TargetToAttackers.Contains(InTarget))
		return false;

	for (const TWeakObjectPtr<ACEnemy_AI>& ai : TargetToAttackers[InTarget])
	{
		if (ai == nullptr)
			continue;

		if (ai == InAttacker)
			return true;
	}

	return false;
}

TArray<TWeakObjectPtr<ACEnemy_AI>> UCBattleManager::GetAttackers(AActor* InTarget) const
{
	if (TargetToAttackers.Contains(InTarget))
		return TargetToAttackers[InTarget];

	return TArray<TWeakObjectPtr<ACEnemy_AI>>();
}

bool UCBattleManager::IsAttackableToTarget(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	if (!IsValid(InTarget))
	{
		FLog::Log("IsAttackableToTarget - InTarget is invalid!");
		return false;
	}

	if (!IsValid(InAttacker))
	{
		FLog::Log("IsAttackableToTarget - InAttacker is invalid!");
		return false;
	}


	// 등록되지 않은 타겟 
	if (TargetToAttackers.Contains(InTarget) == false)
	{
		FLog::Log("IsAttackableToTarget - No Regist Target");

		// 대상이 테이블에 없기 때문에 false 처리하고 등록해놓는다. 
		RegistBattle(InTarget, InAttacker);

		return false;
	}

	TArray<TWeakObjectPtr<ACEnemy_AI>>& Attackers = TargetToAttackers[InTarget];
	if (Attackers.Num() > MaxAttackersPerTarget)
	{
		for (const TWeakObjectPtr<ACEnemy_AI>& attacker : Attackers)
		{
			if (!attacker.IsValid())
				continue;

			if (attacker == InAttacker)
			{
				UnregistAttacker(InTarget, InAttacker);
				FLog::Log("IsAttackableToTarget - Previous Registed Attacker ");
				return false;
			}
		}

		FLog::Log("IsAttackableToTarget - Over Attacker Count ");
		return false;
	}

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent, ACEnemy_AI>(InAttacker);
	if (behavior == nullptr)
	{
		FLog::Log("IsAttackableToTarget - Attacker is Invalid Behaivor ");
		return false;
	}

	int32 myToken = behavior->GetMyToken();
	if (!TargetToTokenCount.Contains(InTarget))
	{
		FLog::Log("IsAttackableToTarget - TargetToTokenCount No Had");
		return false;
	}

	if (myToken < TargetToTokenCount[InTarget])
	{
		FLog::Log("IsAttackableToTarget - Missed Token" + FString::FromInt(myToken) + " / " + FString::FromInt(TargetToTokenCount[InTarget]));
		SetTokenAttacker(InAttacker);
		return false;
	}


	FLog::Log("IsAttackableToTarget - Catch the Token!");
	return true;
}

