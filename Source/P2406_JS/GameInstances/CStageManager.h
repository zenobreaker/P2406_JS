#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameInstances/SpawnStructures/SpawnStructures.h"
#include "CStageManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWaveCleared, int32, WaveID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStageCleared);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossSpawned_Stage, class ACBoss_AI*, Boss);

UCLASS()
class P2406_JS_API UCStageManager : public UObject
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere, Category = "Stage")
    UDataTable* StageDataTable;

	UPROPERTY(EditAnywhere, Category ="Manager")
	TSubclassOf<class UCSpawnManager> SpawnManagerClass;

public:
	UCStageManager();

public:
	void BeginPlay(); 

public: 
	FStageInfo* GetStageInfo(int32 InStageID);
	void StartStageSelectedStage(); 
	void StartStage(int32 InStageID);
	void StartBossStage(); 
	void SpwanWave(int32 InWaveID);
	void ClearStage();

public:
	UFUNCTION()
	void OnWaveCompleted();

	UFUNCTION()
	void OnBossCompleted();

	UFUNCTION()
	void OnBossSpawned(class ACBoss_AI* Boss);

public:
	FOnWaveCleared OnWaveCleared;
	FOnStageCleared OnStageCleared;
	FOnBossSpawned_Stage OnBossSpawned_Stage;

private:
	class UCSpawnManager* SpawnManager;

private:
	int32 CurrentStageID;
	int32 CurrentWaveIndex;


};
