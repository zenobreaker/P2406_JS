#include "Characters/CBoss_AI.h"
#include "Global.h"

#include "Components/CPatternComponent.h"

ACBoss_AI::ACBoss_AI()
{
	FHelpers::CreateActorComponent<UCPatternComponent>(this, &Pattern, "Pattern");
}
