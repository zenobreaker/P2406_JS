#include "GameInstances/CGameInstance.h"
#include "Global.h"

#include "GameInstances/CSkillManager.h"
#include "GameInstances/CBattleManager.h"
#include "GameInstances/CPatternConditionManager.h"

UCGameInstance::UCGameInstance()
{
	SkillManager = CreateDefaultSubobject<UCSkillManager>(TEXT("SkillManager"));
	BattleManager = CreateDefaultSubobject <UCBattleManager>(L"BattleManager");
	PatternCondition = CreateDefaultSubobject<UCPatternConditionManager>(L"PatternCondition");
}

void UCGameInstance::Init()
{
	Super::Init();
	if(SkillManager != nullptr)
	{

		 // ���⼭ �� �ұ�? 
	}


	PatternCondition->InitConditionData();
}

