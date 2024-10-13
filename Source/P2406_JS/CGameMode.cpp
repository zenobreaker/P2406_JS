#include "CGameMode.h"
#include "Global.h"

ACGameMode::ACGameMode()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, L"/Script/Engine.Blueprint'/Game/Player/BP_CPlayerTest.BP_CPlayerTest_C'");
}