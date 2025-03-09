#include "Characters/CBoss_AI.h"
#include "Global.h"

#include "Components/CPatternComponent.h"

ACBoss_AI::ACBoss_AI()
{
	FHelpers::CreateActorComponent<UCPatternComponent>(this, &Pattern, "Pattern");
}

void ACBoss_AI::Damaged()
{
	Super::Damaged();

	DYNAMIC_EVENT_CALL(OnBossDamage);
}

void ACBoss_AI::OnHealthPointChanged(float InHealth, float InMaxHealth)
{
	Super::OnHealthPointChanged(InHealth, InMaxHealth);

	DYNAMIC_EVENT_CALL_TWO_PARAMS(OnBossHealthChanged, InHealth, InMaxHealth);
}

