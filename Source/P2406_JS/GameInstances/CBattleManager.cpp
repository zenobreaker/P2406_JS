#include "GameInstances/CBattleManager.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UCBattleManager::UCBattleManager()
{

}

void UCBattleManager::RegisterGroup(int32 InGroupID, ACEnemy_AI* InTarget)
{
	FScopeLock Lock(&Mutex); // 멀티스레드 동기화
	if (!GroupAITable.Contains(InGroupID))
		GroupAITable.Add(InGroupID, TArray<ACEnemy_AI*>());

	TArray<ACEnemy_AI*>& ais = GroupAITable[InGroupID];
	if (!ais.Contains(InTarget))
		ais.Add(InTarget);
}

void UCBattleManager::UnregisterGroup(int32 InGroupID, ACEnemy_AI* InTarget)
{
	FScopeLock Lock(&Mutex); // 멀티스레드 동기화
	if (GroupAITable.Contains(InGroupID))
	{
		TArray<ACEnemy_AI*>& ais = GroupAITable[InGroupID];
		ais.Remove(InTarget);

		if (ais.Num() == 0)
			GroupAITable.Remove(InGroupID);
	}
}

ACharacter* UCBattleManager::GetBattleAttackerOfTarget(int32 InGroupID, ACEnemy_AI* InCaller)
{
	CheckNullResult(InCaller, nullptr);
	if (!GroupAITable.Contains(InGroupID))
		return nullptr;

	ACharacter* bestTarget = nullptr;
	float closestDistance = FLT_MAX;

	// 가장 첫 번째 놈 반환
	for (ACEnemy_AI* ai : GroupAITable[InGroupID])
	{
		if (ai == nullptr)
			continue;

		if (ai == InCaller)
			continue;

		UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
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

	for (ACEnemy_AI* ai : GroupAITable[InGroupID])
	{
		if (ai == InInitiator)
			continue;

		if (ai == nullptr)
			continue;

		UCAIBehaviorComponent* behavior = FHelpers::GetComponent< UCAIBehaviorComponent>(ai);
		if (behavior == nullptr)
			continue;
		auto* blackboard = behavior->GetBlackboard();
		if (blackboard == nullptr)
			continue;
		
		bool bCheck = blackboard->GetValueAsBool("bInBattle");
		if (bCheck == true)
			continue; 
		behavior->SetTarget(InTarget);
		FLog::Log(ai->GetName() + " Participanet! ");
	}
}

void UCBattleManager::RegisterAttacker(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	FScopeLock Lock(&Mutex); // 멀티스레드 동기화
	if (!TargetToAttackers.Contains(InTarget))
		TargetToAttackers.Add(InTarget, TArray<ACEnemy_AI*>());

	TArray<ACEnemy_AI*>& attackers = TargetToAttackers[InTarget];
	if (!attackers.Contains(InAttacker))
		attackers.Add(InAttacker);
}

void UCBattleManager::UnregisterAttacker(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	FScopeLock Lock(&Mutex); // 멀티스레드 동기화
	if (TargetToAttackers.Contains(InTarget))
	{
		TArray<ACEnemy_AI*>& attackers = TargetToAttackers[InTarget];
		attackers.Remove(InAttacker);

		if (attackers.Num() == 0)
			TargetToAttackers.Remove(InTarget);
	}
}

bool UCBattleManager::IsBeingAttacked(AActor* InTarget) const
{
	//FScopeLock Lock(&Mutex); // 멀티스레드 동기화
	return TargetToAttackers.Contains(InTarget) && TargetToAttackers[InTarget].Num() > 0;
}

TArray<class ACEnemy_AI*> UCBattleManager::GetAttackers(AActor* InTarget) const
{
	if (TargetToAttackers.Contains(InTarget))
		return TargetToAttackers[InTarget];

	return TArray<ACEnemy_AI*>();
}

