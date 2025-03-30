#include "GameInstances/CGameManager.h"
#include "Global.h"
#include "GameInstances/CStageManager.h"
#include "Characters/CBoss_AI.h"

UCGameManager::UCGameManager()
{
	CurrentState = EGameState::Max; 
}

void UCGameManager::BeginPlay()
{
	bIsGameJoin = false; 
	if (!!StageManagerClass)
	{
		StageManager = NewObject<UCStageManager>(this, StageManagerClass);
		if (!!StageManager)
		{
			StageManager->BeginPlay();
			REGISTER_EVENT_WITH_REPLACE(StageManager, OnStageCleared, this, UCGameManager::OnStageCleared);
			REGISTER_EVENT_WITH_REPLACE(StageManager, OnBossSpawned_Stage, this, UCGameManager::OnBossSpawned);
		}
	}

}

void UCGameManager::StartGame()
{
	CheckTrue(bIsGameJoin);

	SetGameState(EGameState::Start);
}

void UCGameManager::SetGameState(EGameState InState)
{
	CurrentState = InState;
	HandleState();
}

void UCGameManager::HandleState()
{
	if (bDrawDebug)
		return; 

	switch (CurrentState)
	{
	case EGameState::Start:
		bIsGameJoin = true; 
		SetGameState(EGameState::CombatPreparation);
		break;
	case EGameState::BuffSelect:
		break;
	case EGameState::CombatPreparation:
		HandleCombatPerparation();
		break;
	case EGameState::Combat:
		break;
	case EGameState::BossPreparation:
		HandleBossPreparation();
		break;
	case EGameState::BossBattle:
		HandleBossBattle();
		break;
	case EGameState::End:
		break;
	case EGameState::Max:
		break;
	default:
		break;
	}
}

void UCGameManager::HandleCombatPerparation()
{
	CheckNull(StageManager);

	SetGameState(EGameState::Combat);
	bCommbatComplete = false; 
	StageManager->StartStageSelectedStage(); 
}

void UCGameManager::HandleBossPreparation()
{
	//CheckNull(StageManager); 

	//StageManager->StartBossStage(); 
}

void UCGameManager::HandleBossBattle()
{
	CheckNull(StageManager);

	StageManager->StartBossStage();
}



void UCGameManager::OnStageCleared()
{
	bCommbatComplete = true; 
	SetGameState(EGameState::BossPreparation);
}

void UCGameManager::OnStartBossStage()
{
	if (bCommbatComplete == false)
		return; 

	SetGameState(EGameState::BossBattle);
}

void UCGameManager::OnBossStageCleared()
{
	SetGameState(EGameState::End);
}

void UCGameManager::OnBossSpawned(ACBoss_AI* Boss)
{
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnBossSpawned_GM, Boss);
}

