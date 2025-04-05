#include "GameInstances/CStageManager.h"
#include "Global.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/Character.h"
#include "GameInstances/CSpawnManager.h"
#include "Characters/CPlayer.h"

UCStageManager::UCStageManager()
{
	CurrentStageID = -1; 
	CurrentWaveIndex = -1; 
}

void UCStageManager::BeginPlay(UWorld* InWorld)
{
	World = InWorld;

	CurrentStageID = 1;
	if (SpawnManagerClass)
	{
		SpawnManager = NewObject<UCSpawnManager>(this, SpawnManagerClass);
		if (SpawnManager)
		{
			SpawnManager->BeginPlay();
			SpawnManager->OnAllEnemiesDefeated.AddDynamic(this, &UCStageManager::OnWaveCompleted);
			SpawnManager->OnAllBossDefeated.AddDynamic(this, &UCStageManager::OnBossCompleted);
		}
	}
}

FStageInfo* UCStageManager::GetStageInfo(int32 InStageID)
{
	CheckNullResult(StageDataTable, nullptr);

	FString contextString;

	for (auto& RowName : StageDataTable->GetRowNames())
	{
		FStageInfo* info = StageDataTable->FindRow<FStageInfo>(RowName, contextString);
		if (info && info->StageID == InStageID)
			return info;
	}

	return nullptr;
}

void UCStageManager::StartStageSelectedStage()
{
	StartStage(CurrentStageID);
}

void UCStageManager::StartStage(int32 InStageID)
{
	CurrentStageID = InStageID;
	CurrentWaveIndex = 0; 
	SpwanWave(CurrentWaveIndex); 
}

void UCStageManager::StartBossStage()
{
	CheckNull(SpawnManager);

	SpawnManager->SpawnBoss(CurrentStageID);
}

void UCStageManager::SpwanWave(int32 InWaveID)
{
	FStageInfo* StageInfo = GetStageInfo(CurrentStageID);
	if (!StageInfo || InWaveID >= StageInfo->WaveData.Num()) return;

	if (SpawnManager)
	{
		SpawnManager->SpawnActorWithSpawnData(StageInfo->WaveData[InWaveID]);
		REGISTER_EVENT_WITH_REPLACE(SpawnManager, OnBossSpawned_Spawn, this, UCStageManager::OnBossSpawned);
	}
}

void UCStageManager::ClearStage()
{
	if (SpawnManager)
		SpawnManager->ClearEnemies();
}

void UCStageManager::OnWaveCompleted()
{
	OnWaveCleared.Broadcast(CurrentWaveIndex);
	CurrentWaveIndex++;
	FLog::Log("Wave Clear");

	FStageInfo* stageInfo = GetStageInfo(CurrentStageID);
	if (stageInfo && CurrentWaveIndex >= stageInfo->WaveData.Num())
	{
		FLog::Log("Stage Clear");
		DYNAMIC_EVENT_CALL(OnStageCleared);
	}
	else
		SpwanWave(CurrentWaveIndex);
}

void UCStageManager::OnBossCompleted()
{
	FLog::Log("Boss Clear");
	CheckNull(World);

	// 캐릭터를 다시 원위치로 돌려준다. 
	APlayerStart* PlayerStart = Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(World, APlayerStart::StaticClass()));
	if (PlayerStart)
	{
		FVector StartLocation = PlayerStart->GetActorLocation();
		FRotator StartRotation = PlayerStart->GetActorRotation();

		
		ACPlayer * player = Cast<ACPlayer>(UGameplayStatics::GetPlayerCharacter(World, 0));
		CheckNull(player); 

		player->SetActorLocation(StartLocation);
		player->SetActorRotation(StartRotation);

		return;
	}

	//TArray<AActor*> PlayerStarts;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

	//for (AActor* Actor : PlayerStarts)
	//{
	//	APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
	//	if (PlayerStart && PlayerStart->ActorHasTag("BossReturnPoint"))
	//	{
	//		FVector StartLocation = PlayerStart->GetActorLocation();
	//		FRotator StartRotation = PlayerStart->GetActorRotation();
	//		break;
	//	}
	//}

}

void UCStageManager::OnBossSpawned(ACBoss_AI* Boss)
{
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnBossSpawned_Stage, Boss);
}

