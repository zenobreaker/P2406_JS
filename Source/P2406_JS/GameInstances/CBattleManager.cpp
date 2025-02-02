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

// Ÿ���� ����Ͽ� �׷��� ����
void UCBattleManager::RegistGroup(int32 InGroupID, ACEnemy_AI* InMember)
{
	FScopeLock Lock(&Mutex); // ��Ƽ������ ����ȭ
	if (!GroupAITable.Contains(InGroupID))
		GroupAITable.Add(InGroupID, TArray<ACEnemy_AI*>());

	TArray<ACEnemy_AI*>& ais = GroupAITable[InGroupID];
	if (!ais.Contains(InMember))
	{
		ais.Add(InMember);
	}
}

void UCBattleManager::UnregistGroup(int32 InGroupID, ACEnemy_AI* InMember)
{
	FScopeLock Lock(&Mutex); // ��Ƽ������ ����ȭ
	if (GroupAITable.Contains(InGroupID))
	{
		TArray<ACEnemy_AI*>& ais = GroupAITable[InGroupID];
		ais.Remove(InMember);

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

	// ���� ù ��° �� ��ȯ
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

		auto* prevTarget = behavior->GetTarget(); 
		if (prevTarget != nullptr)
			continue; 

		behavior->SetTarget(InTarget);
		FLog::Log(ai->GetName() + " Participanet! ");
	}
}

// �ش� Ÿ���� �׷��� ���
void UCBattleManager::RegistBattle(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	CheckNull(InTarget);
	CheckNull(InAttacker);

	FScopeLock Lock(&Mutex); // ��Ƽ������ ����ȭ
	if (!TargetToAttackers.Contains(InTarget))
	{
		// Ÿ�ٿ��� ������ �����ϵ����̺�Ʈ �߰� 
		ACBaseCharacter* character = Cast<ACBaseCharacter>(InTarget);
		if(character != nullptr)
			REGISTER_EVENT_WITH_REPLACE(character, OnCharacterDead_One, this, UCBattleManager::UnregistTarget);
		
		SetTokenTarget(InTarget);

		TargetToAttackers.Add(InTarget, TArray<ACEnemy_AI*>());
	}

	TArray<ACEnemy_AI*>& attackers = TargetToAttackers[InTarget];
	if (!attackers.Contains(InAttacker))
	{
		// ����� �������� ��� - �����ڰ� ������ �����ϵ��� 
		REGISTER_EVENT_WITH_REPLACE(InAttacker, OnCharacterDead_One, this, UCBattleManager::UnregistAttacker);
		// �׷쿡 �߰� �� �� 
		SetTokenAttacker(InAttacker);

		attackers.Add(InAttacker);
	}
}

void UCBattleManager::UnregistBattle(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	FScopeLock Lock(&Mutex); // ��Ƽ������ ����ȭ

	UnregistAttacker(InTarget, InAttacker);
}

// ������ ��� ���� 
void UCBattleManager::UnregistAttacker(ACharacter* InAttacker)
{
	CheckNull(InAttacker);

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(InAttacker);
	CheckNull(ai);

	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(ai);
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

// Ÿ�� ��� ���� 
void UCBattleManager::UnregistTarget(ACharacter* InTarget)
{
	CheckNull(InTarget);

	// �� ����� ��ϵǾ� ���� �ʴٸ�?
	if (!TargetToAttackers.Contains(InTarget))
	{
		return;
	}

	//TODO: ���� �� Target Ű���� ���� Value �鿡�� �� �׾����ϱ� �ʳ� �� ���� ����

	TargetToTokenCount.Remove(InTarget);
	TargetToAttackers.Remove(InTarget);
}


void UCBattleManager::SetTokenAttacker(ACEnemy_AI* InAttacker)
{
	CheckNull(InAttacker);
	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(InAttacker);
	CheckNull(behavior);

	int32 tokenValue = FMath::RandRange(0, MaxTokenValue);
	behavior->SetMyToken(tokenValue);
}

void UCBattleManager::SetTokenTarget(AActor* InTarget)
{
	CheckNull(InTarget);
	CheckTrue(TargetToTokenCount.Contains(InTarget));

	// �߰� �� ���� ���� ���� ��ū ���� 
	// �ִ� ���� ���� �ʰ� ���ݸ� �غ�
	int32 token = FMath::RandRange(0, MaxTokenValue / 2);

	TargetToTokenCount.Add(InTarget, token);
}

bool UCBattleManager::IsContainFromAttackers(AActor* InTarget, class ACEnemy_AI* InAttacker) const
{
	//FScopeLock Lock(&Mutex); // ��Ƽ������ ����ȭ
	CheckNullResult(InTarget, false);
	CheckNullResult(InAttacker, false);

	if (!TargetToAttackers.Contains(InTarget))
		return false; 
	
	for (const ACEnemy_AI* ai : TargetToAttackers[InTarget])
	{
		if (ai == nullptr)
			continue;

		if (ai == InAttacker)
			return true; 
	}

	return false; 
}

TArray<class ACEnemy_AI*> UCBattleManager::GetAttackers(AActor* InTarget) const
{
	if (TargetToAttackers.Contains(InTarget))
		return TargetToAttackers[InTarget];

	return TArray<ACEnemy_AI*>();
}

bool UCBattleManager::IsAttackableToTarget(AActor* InTarget, ACEnemy_AI* InAttacker)
{
	// ��ϵ��� ���� Ÿ�� 
	if (TargetToAttackers.Contains(InTarget) == false)
	{
		FLog::Log("IsAttackableToTarget - No Regist Target");
		
		// ����� ���̺� ���� ������ false ó���ϰ� ����س��´�. 
		RegistBattle(InTarget, InAttacker);

		return false;
	}

	// �̹� �� ���� ������ �� �ִ� ���� �ο����� ���� 
	if (TargetToAttackers[InTarget].Num() > MaxAttackersPerTarget)
	{
		// �ٵ� �� �ο� �� �߿� ���� �̹� �ִٸ� ����� �����ϰ� ���� ��ȸ�� �ѱ��.
		for (ACEnemy_AI* attacker : TargetToAttackers[InTarget])
		{
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

	bool bAttack = true;
	/*for (ACEnemy_AI* attacker : TargetToAttackers[InTarget])
	{
		if (attacker == nullptr)
		{
			FLog::Log("IsAttackableToTarget - No Regist attacker");
			return false;
		}

		UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(attacker);
		if (behavior == nullptr)
			continue; 
		

		if (behavior->IsActionMode())
		{
			FLog::Log("IsAttackableToTarget - ");
			return false;
		}

		break;
	}*/


	UCAIBehaviorComponent* behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(InAttacker);
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
	return bAttack;
}

