#include "CGameMode.h"
#include "EngineUtils.h"
#include "Global.h"
#include "GameInstances/CGameInstance.h"
#include "GameInstances/CSkillManager.h"
#include "Widgets/CUserWidget_SkillHUD.h"

#include "Characters/CEnemy_AI.h"
#include "Characters/CPlayerController.h"
#include "Components/CConditionComponent.h"

ACGameMode::ACGameMode()
{
	FHelpers::GetClass<APawn>(&DefaultPawnClass, "/Script/Engine.Blueprint'/Game/Player/BP_CPlayer2.BP_CPlayer2_C'");
	PlayerControllerClass = ACPlayerController::StaticClass(); 


	PrimaryActorTick.bCanEverTick = true;
}

void ACGameMode::BeginPlay()
{
	GameInstance = Cast<UCGameInstance>(GetGameInstance());
	
	if (GameInstance != nullptr)
		FLog::Log("Success Find the GameInstance ");
}

void ACGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GameInstance != nullptr && GameInstance->SkillManager != nullptr)
	{
		GameInstance->SkillManager->UpdateAllCooldown(DeltaSeconds);
	}
}

void ACGameMode::SubscribeToSkillEvents(UCUserWidget_SkillHUD* SkillHUD)
{
	CheckNull(SkillHUD);
	CheckNull(GameInstance);
	CheckNull(GameInstance->SkillManager);

	GameInstance->SkillManager->OnUpdateCooldown.AddDynamic(SkillHUD, &UCUserWidget_SkillHUD::OnUpdateCooldown);
}

void ACGameMode::OrderToAllActorDown()
{
	FLog::Log("All Down !! ");

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* actor = *It;
		if(actor)
		{
			UCConditionComponent* condition = FHelpers::GetComponent<UCConditionComponent>(actor);

			if(condition)
			{
				condition->AddDownCondition();
			}
		}
	}
}

void ACGameMode::ToggleEnemyUI()
{
	bEnemyUiHide = !bEnemyUiHide; 
	FString hideResultText = bEnemyUiHide == true ? "True" : "False";

	FLog::Log("Enemy UI Toggle" + hideResultText);

	for (TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* actor = *It;
		if (actor)
		{
			ACEnemy_AI* ai = Cast<ACEnemy_AI>(actor); 
			if (!!ai)
				ai->OnToggleEnemyUI(bEnemyUiHide);
		}
	}
}
