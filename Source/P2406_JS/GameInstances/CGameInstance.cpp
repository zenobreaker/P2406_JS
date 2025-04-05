#include "GameInstances/CGameInstance.h"
#include "Global.h"

#include "GameInstances/CSkillManager.h"
#include "GameInstances/CBattleManager.h"
#include "GameInstances/CPatternConditionManager.h"
#include "GameInstances/CGameManager.h"
#include "GameInstances/CBuffUIManager.h"
#include "Widgets/CUserWidget_BuffSelectHUD.h"

UCGameInstance::UCGameInstance()
{
	SkillManagerClass = UCSkillManager::StaticClass();
	BattleManagerClass = UCBattleManager::StaticClass();
	PatternConditionClass = UCPatternConditionManager::StaticClass();
}

void UCGameInstance::Init()
{
	Super::Init();

	if (PatternConditionClass != nullptr)
	{
		PatternCondition = NewObject<UCPatternConditionManager>(this, PatternConditionClass);
		if (PatternCondition != nullptr)
			PatternCondition->InitConditionData();
	}


	if (SkillManagerClass != nullptr)
	{
		SkillManager = NewObject<UCSkillManager>(this, SkillManagerClass);
	}

	if (BattleManagerClass != nullptr)
	{
		BattleManager = NewObject<UCBattleManager>(this, BattleManagerClass);
	}


	if (BuffUIManagerClass != nullptr)
	{
		BuffUIManager = NewObject<UCBuffUIManager>(this, BuffUIManagerClass);
		if (!!BuffUIManager)
		{
			UCBuffUIManager* buffUIManager = BuffUIManagerClass->GetDefaultObject<UCBuffUIManager>();
			CheckNull(buffUIManager);

			BuffUIManager->SetBuffHUDClass(buffUIManager->GetBuffHUDClass().Get());
			BuffUIManager->BeginPlay(GetWorld());
		}
	}

	if (GameManagerClass == nullptr)
	{
		FHelpers::GetAssetAsync<UCGameManager>(&GameManager, "/Script/Engine.Blueprint'/Game/Managers/BP_CGameManager.BP_CGameManager'",
			[this]()
			{
				if (!!GameManager)
					GameManager->BeginPlay(GetWorld());
			});
	}
	else
	{
		GameManager = NewObject<UCGameManager>(this, GameManagerClass);
		if (GameManager != nullptr)
			GameManager->BeginPlay(GetWorld());
	}

	if (BuffUIManager != nullptr && GameManager != nullptr)
	{
		REGISTER_EVENT_WITH_REPLACE(BuffUIManager, OnShowedUIBuffList, GameManager, UCGameManager::OnShowBuffList);
		REGISTER_EVENT_WITH_REPLACE(BuffUIManager, OnHidedUIBuffList, GameManager, UCGameManager::OnHideBuffList);
	}
}

