#include "Characters/CBoss_AI.h"
#include "Global.h"

#include "Components/CPatternComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CHealthPointComponent.h"

ACBoss_AI::ACBoss_AI()
{
	FHelpers::CreateActorComponent<UCPatternComponent>(this, &Pattern, "Pattern");
}

void ACBoss_AI::BeginPlay()
{
	Super::BeginPlay();

	if (bSuperArmor && !!Condition)
	{
		Condition->AddSuperArmorCondition();
	}

	CurrentPhase = 1; 
	StartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
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

void ACBoss_AI::CheckPhaseTransition()
{
	CheckNull(BossPhaseData);
	CheckNull(HealthPoint);
	CheckFalse(BossPhaseData->PhaseConditions.Num() > 0);

	int32 nextPhase = CurrentPhase;
	for (int32 i = 0; i < BossPhaseData->PhaseConditions.Num(); i++)
	{
		if (BossPhaseData->PhaseConditions[i].ValidPhase != CurrentPhase)
			continue; 

		switch (BossPhaseData->PhaseConditions[i].TransitionType)
		{
			case EPhaseTransitionType::HPThreshold:
				if (HealthPoint->GetHealth() <= BossPhaseData->PhaseConditions[i].HPThreshold)
					nextPhase++;
				break;

			case EPhaseTransitionType::TimeElapsed:
			{
				CurrentTime = UGameplayStatics::GetTimeSeconds(GetWorld());
				float ElapsedTime = CurrentTime - StartTime;
				if (ElapsedTime >= BossPhaseData->PhaseConditions[i].TimeThreshold)
					nextPhase++; 
			}
				break;

			case EPhaseTransitionType::CustomCondition:
				//TODO: ... 
				break;
		}
	}

	if (nextPhase > CurrentPhase)
	{
		nextPhase = CurrentPhase;
		SetNextPhase(++nextPhase);
	}
}

void ACBoss_AI::SetNextPhase(int32 InNextPhase)
{
	CurrentPhase = FMath::Clamp(InNextPhase, 0, BossPhaseData->BossMaxPhase);

	DYNAMIC_EVENT_CALL_ONE_PARAM(OnBossPhaseUpdated, CurrentPhase); 
}

