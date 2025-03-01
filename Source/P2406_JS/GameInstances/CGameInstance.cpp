#include "GameInstances/CGameInstance.h"
#include "Global.h"
#include "GameInstances/CSkillManager.h"
#include "GameInstances/CBattleManager.h"
#include "GameInstances/CPatternConditionManager.h"

UCGameInstance::UCGameInstance()
{
	SkillManager = CreateDefaultSubobject<UCSkillManager>(TEXT("SkillManager"));
	BattleManager = CreateDefaultSubobject <UCBattleManager>(L"BattleManager");
}

void UCGameInstance::Init()
{
	Super::Init();
	if(SkillManager != nullptr)
	{

		 // 여기서 뭘 할까? 
	}
	
	UCPatternConditionManager::Get()->InitConditionData();
}

