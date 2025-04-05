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
	SkillManager = CreateDefaultSubobject<UCSkillManager>(TEXT("SkillManager"));
	BattleManager = CreateDefaultSubobject <UCBattleManager>(L"BattleManager");
	//PatternCondition = CreateDefaultSubobject<UCPatternConditionManager>(L"PatternCondition");
	//SpawnManager = CreateDefaultSubobject<UCSpawnManager>(L"SpawnManager"); 
}

void UCGameInstance::Init()
{
	Super::Init();

	FHelpers::GetAssetAsync<UCPatternConditionManager>(&PatternCondition, "",
		[this]()
		{
			if (!!PatternCondition)
				PatternCondition->InitConditionData();
		});


	if (!!SkillManagerClass)
	{
		SkillManager = NewObject<UCSkillManager>(this, SkillManagerClass);
	}

	if (!!BattleManagerClass)
	{
		BattleManager = NewObject<UCBattleManager>(this, BattleManagerClass);
	}


	if (!!BuffUIManagerClass)
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

