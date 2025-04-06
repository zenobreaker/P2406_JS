#include "GameInstances/CGameManager.h"
#include "Global.h"

#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "CineCameraActor.h"

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

void UCGameManager::BeginPlay(UWorld* InWorld)
{
	MyWorld = InWorld;
	bIsGameJoin = false; 
	if (StageManagerClass != nullptr)
	{
		StageManager = NewObject<UCStageManager>(this, StageManagerClass);
		if (IsValid(StageManager))
		{
			StageManager->BeginPlay(InWorld);
			REGISTER_EVENT_WITH_REPLACE(StageManager, OnStageCleared, this, UCGameManager::OnStageCleared);
			REGISTER_EVENT_WITH_REPLACE(StageManager, OnBossSpawned_Stage, this, UCGameManager::OnBossSpawned);
		}
	}

	if (BuffManagerClass != nullptr)
	{
		BuffManager = NewObject<UCBuffManager>(this, BuffManagerClass);
		if (IsValid(BuffManager))
			BuffManager->BeginPlay();
	}

	ULevelSequence* levelSequence = nullptr;
	levelSequence = LoadObject<ULevelSequence>(nullptr, L"/Script/LevelSequence.LevelSequence'/Game/SQ1.SQ1'");
	if (levelSequence != nullptr)
	{
		FMovieSceneSequencePlaybackSettings PlaybackSettings;
		PlaybackSettings.bAutoPlay = false; // 시작하자마자 재생

		ALevelSequenceActor* outActor;
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
			InWorld,
			levelSequence,
			PlaybackSettings,
			outActor
		);

		if(SequencePlayer != nullptr)
			SequencePlayer->OnFinished.AddDynamic(this, &UCGameManager::OnCinematicFinished);
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

		//SetGameState(EGameFlowState::BuffSelect);
		HandleStartGeme();
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

void UCGameManager::HandleStartGeme()
{
	bIsGameJoin = true;
	if (bIsIntroSkip == true)
	{
		SetGameState(EGameFlowState::BuffSelect);
		return; 
	}

	if (bIsFirstCinematic == true)
	{
		SetGameState(EGameFlowState::BuffSelect);

		return;
	}

	if (SequencePlayer != nullptr)
	{
		ACPlayer* player = Cast<ACPlayer>(MyWorld->GetFirstPlayerController()->GetCharacter());
		if (player != nullptr)
			player->OnHiddenUI();


		bIsFirstCinematic = true; 
		SequencePlayer->Play(); 

		return; 
	}

	SetGameState(EGameFlowState::BuffSelect);
}

void UCGameManager::HandleBuffSelect()
{
	if (bIsBuffSkip == true)
	{
		SetGameState(EGameFlowState::CombatPreparation);
		return; 
	}

	CheckNull(BuffManager); 
	CheckNull(MyWorld);
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(MyWorld));
	CheckNull(instance); 
	CheckNull(instance->BuffUIManager);

	FLog::Log(" Buff Select ");

	BuffManager->CreatRandomBuffList(3, Buffs);

	instance->BuffUIManager->ShowBuffSelection(Buffs);
}

void UCGameManager::HandleCombatPerparation()
{
	if (bIsCombatSkip == true)
	{
		SetGameState(EGameFlowState::BossPreparation);
		return; 
	}

	CheckNull(StageManager);

	SetGameState(EGameFlowState::Combat);
	bCommbatComplete = false; 
	StageManager->StartStageSelectedStage(); 
}

void UCGameManager::HandleBossPreparation()
{
	if (bIsBossSkip == true)
	{
		SetGameState(EGameFlowState::End);
		return;
	}
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
	APlayerController* PlayerController = MyWorld->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}
}



void UCGameManager::SendSelctedBuffFlow()
{	
	CheckNull(BuffManager);
	CheckNull(MyWorld);
	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(MyWorld));
	CheckNull(instance);
	CheckNull(instance->BuffUIManager); 

	// 1. 버프 UI 매니저로부터 선택한 버프 가져오기 
	FStatBuff statBuff =  instance->BuffUIManager->GetSelectedStatBuff();

	//2. 버프 매니저한테 버프 적용하라고 전달 
	ACPlayer* player = Cast<ACPlayer>(MyWorld->GetFirstPlayerController()->GetCharacter());
	BuffManager->ApplyBuffsToPlayer(player, statBuff);
}



void UCGameManager::OnHideBuffList()
{
	APlayerController* PlayerController = MyWorld->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	SendSelctedBuffFlow();

	SetGameState(EGameFlowState::CombatPreparation);
}

void UCGameManager::OnCinematicFinished()
{
	SetGameState(EGameFlowState::BuffSelect);

	// 여기서 입력 가능하게 풀어준다
	APlayerController* PC = UGameplayStatics::GetPlayerController(MyWorld, 0);
	if (PC)
	{
		PC->SetCinematicMode(false, false, false, true, true);
	}

	CheckNull(PC->GetCharacter());
	ACPlayer* player = Cast<ACPlayer>(PC->GetCharacter());
	if (player != nullptr)
		player->OnVisibilityUI();

}

