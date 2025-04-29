#include "GameInstances/CBuffManager.h"
#include "Global.h"

#include "Characters/CPlayer.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CBuffComponent.h"
#include "Components/CStatComponent.h"

void UCBuffManager::BeginPlay()
{
	BuffDatabase.Empty(); 

	BuffDatabase.Add(FStatBuff("Buff_AttackUp_1",ECharStatType::Attack, 10.0f));
	BuffDatabase.Add(FStatBuff("Buff_AttackUp_2",ECharStatType::Attack, 25.0f));
	BuffDatabase.Add(FStatBuff("Buff_AttackUp_3",ECharStatType::Attack, 50.0f));
	BuffDatabase.Add(FStatBuff("Buff_AttackSpeedUp_1",ECharStatType::AttackSpeed, 0.1f));
	BuffDatabase.Add(FStatBuff("Buff_AttackSpeedUp_2",ECharStatType::AttackSpeed, 0.15f));
	BuffDatabase.Add(FStatBuff("Buff_AttackSpeedUp_3",ECharStatType::AttackSpeed, 0.2f));
	BuffDatabase.Add(FStatBuff("Buff_DefenseUp_1",ECharStatType::Defense, 5.0f));
	BuffDatabase.Add(FStatBuff("Buff_DefenseUp_2",ECharStatType::Defense, 10.0f));
	BuffDatabase.Add(FStatBuff("Buff_DefenseUp_3",ECharStatType::Defense, 20.0f));
	BuffDatabase.Add(FStatBuff("Buff_CritRateUp_1",ECharStatType::CriticalRate, 0.15f));
	BuffDatabase.Add(FStatBuff("Buff_CritRateUp_2",ECharStatType::CriticalRate, 0.20f));
	BuffDatabase.Add(FStatBuff("Buff_CritRateUp_3",ECharStatType::CriticalRate, 0.30f));
	BuffDatabase.Add(FStatBuff("Buff_CritDmg_1",ECharStatType::CriticalDamage, 0.1f));
	BuffDatabase.Add(FStatBuff("Buff_CritDmg_2",ECharStatType::CriticalDamage, 0.15f));
	BuffDatabase.Add(FStatBuff("Buff_CritDmg_3",ECharStatType::CriticalDamage, 0.20f));
	BuffDatabase.Add(FStatBuff("Buff_SpeedUp_1",ECharStatType::Speed, 0.1f));
	BuffDatabase.Add(FStatBuff("Buff_SpeedUp_2",ECharStatType::Speed, 0.2f));
	BuffDatabase.Add(FStatBuff("Buff_SpeedUp_3",ECharStatType::Speed, 0.3f));
}

void UCBuffManager::CreatRandomBuffList(int32 InCount, TArray<FStatBuff>& InBuffs)
{
	InBuffs.Empty();

	if (BuffDatabase.Num() == 0) return;


	// 1. 전체 버프 리스트를 복사
	TArray<FStatBuff> TempBuffs = BuffDatabase;

	// 2. Fisher-Yates Shuffle을 부분적으로 수행
	int32 MaxCount = FMath::Min(InCount, BuffDatabase.Num());
	for (int32 i = 0; i < MaxCount; i++)
	{
		int32 RandomIdx = FMath::RandRange(i, TempBuffs.Num() - 1);
		TempBuffs.Swap(i, RandomIdx);
		if (TempBuffs.IsValidIndex(i)) // 유효한 인덱스인지 확인
		{
			InBuffs.Add(TempBuffs[i]);
		}
		else
		{
			// 로그를 남겨서 디버깅할 수 있게
			UE_LOG(LogTemp, Warning, TEXT("Invalid index at %d"), i);
		}
	}
}

void UCBuffManager::ApplyBuffsToPlayer(ACPlayer* InPlayer, const FStatBuff& InBuff)
{
	CheckNull(InPlayer); 
	UCBuffComponent* buff = FHelpers::GetComponent<UCBuffComponent>(InPlayer);
	CheckNull(buff);
	
	PlayerBuffs.Add(InBuff);

	buff->ApplyBuff(InBuff);
}


void UCBuffManager::RemoveBuffsToPlayer(ACPlayer* InPlayer, const FStatBuff& InBuff)
{
	CheckNull(InPlayer);
	UCBuffComponent* buff = FHelpers::GetComponent<UCBuffComponent>(InPlayer);
	CheckNull(buff);

	int32 index = PlayerBuffs.IndexOfByPredicate([&](const FStatBuff& Buff)
		{
			return Buff.StatType == InBuff.StatType && Buff.Value == InBuff.Value;
		});

	CheckTrue(index == INDEX_NONE);

	buff->RemoveBuff(InBuff);
	PlayerBuffs.RemoveAt(index);
}

//void UCBuffManager::RemoveBuffFromEnemy(ACEnemy_AI* InEnemy,  const FStatBuff& InBuff)
//{
//	if (!EnemyBuffs.Contains(InEnemy))
//	{
//		EnemyBuffs.Add(InEnemy, TArray<FStatBuff>());
//	}
//	EnemyBuffs[InEnemy].Add(InBuff);
//}
//
//void UCBuffManager::ClearEnemyBuffs(ACEnemy_AI* InEnemy)
//{
//	EnemyBuffs.Remove(InEnemy);
//}
