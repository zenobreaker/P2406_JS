#include "GameInstances/CSpawnManager.h"
#include "Global.h"

#include "Characters/CBaseCharacter.h"
#include "Characters/CEnemy_AI.h"
#include "Characters/CBoss_AI.h"


void UCSpawnManager::BeginPlay(UWorld* InWorld)
{
	World = InWorld;
	CheckFalse(SpawnEnemies.Num() > 0);

	//for (TSubclassOf<ACEnemy_AI> EnemyClass : SpawnEnemies)
	//{
	//	if (EnemyClass == nullptr)
	//		continue;

	//	UClass* enemyUClass = EnemyClass.Get();
	//	if (enemyUClass == nullptr)
	//		continue;

	//	uint32 uid = enemyUClass->GetDefaultObject<ACEnemy_AI>()->GetUID();
	//	if (uid == UINT32_MAX)
	//		continue; 

	//	if (SpawnEnemyTable.Contains(uid)) // Check if UID already exists in the map
	//	{
	//		FLog::Log("Duplicate UID found: " + FString::FromInt(uid));
	//		continue;
	//	}
	//	
	//	//SpawnEnemyTable.Add(uid, EnemyClass);
	//	FLog::Log("Spawn Table id : " + FString::FromInt(uid));
	//}
}


void UCSpawnManager::SpawnActorWithSpawnData(const FStageWaveInfo& WaveInfo)
{
	CheckNull(World);
	int32 count = 0;
	for (const FMonsterSpawnInfo& MonsterInfo : WaveInfo.Monsters)
	{
		if (MonsterInfo.MonsterClass->IsChildOf<ACBoss_AI>())
		{
			if (SpawnBossTable.Contains(WaveInfo.StageID) == false)
			{
				SpawnBossTable.Add(WaveInfo.StageID, TArray<FMonsterSpawnInfo>());
			}

			SpawnBossTable[WaveInfo.StageID].Add(MonsterInfo);
			continue;
		}

		if (MonsterInfo.MonsterClass == nullptr)
			continue;

		ACEnemy_AI* SpawnedMonster = World->SpawnActor<ACEnemy_AI>(MonsterInfo.MonsterClass, MonsterInfo.SpawnLocation, MonsterInfo.SpawnRotation);
		if (!!SpawnedMonster)
		{
			count++;
			EnemeyList.AddUnique(SpawnedMonster);
			SpawnedMonster->OnCharacterDead.AddDynamic(this, &UCSpawnManager::OnEnemyDefeated);

		}
	}

	RemainingEnemies = count;
}

void UCSpawnManager::ClearEnemies()
{
	CheckTrue(EnemeyList.IsEmpty());

	for (ACEnemy_AI* ai : EnemeyList)
	{
		ai->Destroy();
	}

	EnemeyList.Empty();
}

void UCSpawnManager::SpawnBoss(int32 InStageID)
{
	CheckTrue(SpawnBossTable.IsEmpty());
	CheckFalse(SpawnBossTable.Contains(InStageID));
	CheckFalse(SpawnBossTable[InStageID].Num() > 0);

	int32 count = 0;
	for (auto& boss : SpawnBossTable[InStageID])
	{
		ACBoss_AI* bossMonster = GetWorld()->SpawnActor<ACBoss_AI>(boss.MonsterClass, boss.SpawnLocation, boss.SpawnRotation);
		if (!!bossMonster)
		{
			count++;
			BossList.AddUnique(bossMonster);
			bossMonster->OnCharacterDead.AddDynamic(this, &UCSpawnManager::OnBossDefeatted);
			bossMonster->OnCharacterEndDead.AddDynamic(this, &UCSpawnManager::OnBossDeathEventFinished);
			OnBossSpawned(bossMonster);
		}
	}
	RemainingBossDeaathEvent = count;
	RemainingEnemies = count;
}



void UCSpawnManager::OnEnemyDefeated()
{
	RemainingEnemies--;

	if (RemainingEnemies <= 0)
	{
		DYNAMIC_EVENT_CALL(OnAllEnemiesDefeated);
	}
}

void UCSpawnManager::OnBossDefeatted()
{
	RemainingEnemies--;


}

void UCSpawnManager::OnBossSpawned(ACBoss_AI* Boss)
{
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnBossSpawned_Spawn, Boss);
}

void UCSpawnManager::OnBossDeathEventFinished()
{
	RemainingBossDeaathEvent--;

	if (RemainingEnemies <= 0 && RemainingBossDeaathEvent <= 0)
	{
		DYNAMIC_EVENT_CALL(OnAllBossDefeated);
	}
}
