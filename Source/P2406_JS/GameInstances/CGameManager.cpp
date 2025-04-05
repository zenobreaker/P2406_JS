#include "GameInstances/CGameManager.h"
#include "Global.h"

#include "Characters/CBoss_AI.h"
#include "Characters/CPlayer.h"
#include "GameInstances/CGameInstance.h"
#include "GameInstances/CStageManager.h"
#include "GameInstances/CBuffManager.h"
#include "GameInstances/CBuffUIManager.h"

UCGameManager::UCGameManager()
{
	CurrentState = EGameFlowState::Max; 

	BuffManagerClass = UCBuffManager::StaticClass();
}

void UCGameManager::BeginPlay()
{
	bIsGameJoin = false; 
	if (!!StageManagerClass)
	{
		StageManager = NewObject<UCStageManager>(this, StageManagerClass);
		if (IsValid(StageManager))
		{
			StageManager->BeginPlay();
			REGISTER_EVENT_WITH_REPLACE(StageManager, OnStageCleared, this, UCGameManager::OnStageCleared);
			REGISTER_EVENT_WITH_REPLACE(StageManager, OnBossSpawned_Stage, this, UCGameManager::OnBossSpawned);
		}
	}

	if (!!BuffManagerClass)
	{
		BuffManager = NewObject<UCBuffManager>(this, BuffManagerClass);
		if (IsValid(BuffManager))
			BuffManager->BeginPlay();
	}
}

void UCGameManager::StartGame()
{
	CheckTrue(bIsGameJoin);

	SetGameState(EGameFlowState::Start);
}

void UCGameManager::SetGameState(EGameFlowState InState)
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
	case EGameFlowState::Start:
		bIsGameJoin = true; 
		SetGameState(EGameFlowState::BuffSelect);
		break;
	case EGameFlowState::BuffSelect:
		HandleBuffSelect();
		break;
	case EGameFlowState::CombatPreparation:
		HandleCombatPerparation();
		break;
	case EGameFlowState::Combat:
		break;
	case EGameFlowState::BossPreparation:
		HandleBossPreparation();
		break;
	case EGameFlowState::BossBattle:
		HandleBossBattle();
		break;
	case EGameFlowState::End:
		break;
	case EGameFlowState::Max:
		break;
	default:
		break;
	}
}

void UCGameManager::HandleBuffSelect()
{
	CheckNull(BuffManager); 
	CheckNull(GetWorld());
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance); 
	CheckNull(instance->BuffUIManager);

	FLog::Log(" Buff Select ");

	BuffManager->CreatRandomBuffList(3, Buffs);

	instance->BuffUIManager->ShowBuffSelection(Buffs);
}

void UCGameManager::HandleCombatPerparation()
{
	CheckNull(StageManager);

	SetGameState(EGameFlowState::Combat);
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
	SetGameState(EGameFlowState::BossPreparation);
}

void UCGameManager::OnStartBossStage()
{
	if (bCommbatComplete == false)
		return; 

	SetGameState(EGameFlowState::BossBattle);
}

void UCGameManager::OnBossStageCleared()
{
	SetGameState(EGameFlowState::End);
}

void UCGameManager::OnBossSpawned(ACBoss_AI* Boss)
{
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnBossSpawned_GM, Boss);
}

void UCGameManager::OnShowBuffList()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}
}



void UCGameManager::SendSelctedBuffFlow()
{	
	CheckNull(BuffManager);
	CheckNull(GetWorld());
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	CheckNull(instance);
	CheckNull(instance->BuffUIManager); 

	// 1. 버프 UI 매니저로부터 선택한 버프 가져오기 
	FStatBuff statBuff =  instance->BuffUIManager->GetSelectedStatBuff();

	//2. 버프 매니저한테 버프 적용하라고 전달 
	ACPlayer* player = Cast<ACPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	BuffManager->ApplyBuffsToPlayer(player, statBuff);
}



void UCGameManager::OnHideBuffList()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	SendSelctedBuffFlow();

	SetGameState(EGameFlowState::CombatPreparation);
}

