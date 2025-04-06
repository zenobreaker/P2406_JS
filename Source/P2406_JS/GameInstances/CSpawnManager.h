#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameInstances/SpawnStructures/SpawnStructures.h"
#include "CSpawnManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllEnemiesDefeated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAllBossDefeated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossSpawned_Spawn, class ACBoss_AI*, Boss);


UCLASS(Blueprintable)
class P2406_JS_API UCSpawnManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Spawn Point")
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Spawn Enemies")
	TArray<TSubclassOf<class ACEnemy_AI>> SpawnEnemies;


public:
	void BeginPlay(UWorld* InWorld);

public:
	void SpawnActorWithSpawnData(const struct FStageWaveInfo& WaveInfo);
	void ClearEnemies();
	void SpawnBoss(int32 InStageID);

public:
	UFUNCTION()
	void OnEnemyDefeated();

	UFUNCTION()
	void OnBossDefeatted();

	UFUNCTION()
	void OnBossSpawned(class ACBoss_AI* Boss);
	UFUNCTION()
	void OnBossDeathEventFinished();

public:
	FOnAllEnemiesDefeated OnAllEnemiesDefeated; 
	FOnAllBossDefeated OnAllBossDefeated;
	FOnBossSpawned_Spawn OnBossSpawned_Spawn;

private:
	//TMap<uint32, TSubclassOf<class ACEnemy_AI>> SpawnEnemyTable;
	TMap<int32, TArray<FMonsterSpawnInfo>> SpawnBossTable;
	UWorld* World; 
private:
	int32 spawnCount;
	int32 RemainingEnemies;
	int32 RemainingBossDeaathEvent = 0;
	TArray<class ACEnemy_AI*> EnemeyList;
	TArray<class ACBoss_AI*> BossList; 
};
