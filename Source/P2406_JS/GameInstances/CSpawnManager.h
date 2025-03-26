#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSpawnManager.generated.h"


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
	void BeginPlay();

public:
	void SpawnActorWithRandomSpawnPoints(int32 InID);

private:
	TMap<uint32, TSubclassOf<class ACEnemy_AI>> SpawnEnemyTable;	

private:
	int32 spawnCount; 

};
