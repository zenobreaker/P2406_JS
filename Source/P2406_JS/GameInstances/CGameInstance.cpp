#include "GameInstances/CGameInstance.h"
#include "Global.h"
#include "GameInstances/CSkillManager.h"

UCGameInstance::UCGameInstance()
{
	SkillManager = CreateDefaultSubobject<UCSkillManager>(TEXT("SkillManager"));
}

void UCGameInstance::Init()
{
	Super::Init();
	if(SkillManager != nullptr)
	{

		 // ���⼭ �� �ұ�? 
	}
}

