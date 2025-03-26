#include "GameInstances/CSpawnManager.h"
#include "Global.h"

#include "Characters/CBaseCharacter.h"
#include "Characters/CEnemy_AI.h"

void UCSpawnManager::BeginPlay()
{
	CheckFalse(SpawnEnemies.Num() > 0);

	for (TSubclassOf<ACEnemy_AI> EnemyClass : SpawnEnemies)
	{
		if (EnemyClass == nullptr)
			continue;

		UClass* enemyUClass = EnemyClass.Get();
		if (enemyUClass == nullptr)
			continue;

		uint32 uid = enemyUClass->GetDefaultObject<ACEnemy_AI>()->GetUID();
		if (uid == UINT32_MAX)
			continue; 

		if (SpawnEnemyTable.Contains(uid)) // Check if UID already exists in the map
		{
			FLog::Log("Duplicate UID found: " + FString::FromInt(uid));
			continue;
		}
		
		SpawnEnemyTable.Add(uid, EnemyClass);
		FLog::Log("Spawn Table id : " + FString::FromInt(uid));
	}
}

void UCSpawnManager::SpawnActorWithRandomSpawnPoints(int32 InID)
{

}
