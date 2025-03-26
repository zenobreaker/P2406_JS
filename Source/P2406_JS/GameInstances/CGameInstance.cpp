#include "GameInstances/CGameInstance.h"
#include "Global.h"

#include "GameInstances/CSkillManager.h"
#include "GameInstances/CBattleManager.h"
#include "GameInstances/CPatternConditionManager.h"
#include "GameInstances/CSpawnManager.h"

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

	
	if (SpawnManagerClass == nullptr)
	{
		FHelpers::GetAssetAsync<UCSpawnManager>(&SpawnManager, "/Script/Engine.Blueprint'/Game/Managers/BP_CSpawnManager.BP_CSpawnManager'",
			[this]()
			{
				if (!!SpawnManager)
					SpawnManager->BeginPlay();
			});
	}
	else
	{
		SpawnManager = NewObject<UCSpawnManager>(this, SpawnManagerClass);
		if(SpawnManager != nullptr)
			SpawnManager->BeginPlay();
	}
}

