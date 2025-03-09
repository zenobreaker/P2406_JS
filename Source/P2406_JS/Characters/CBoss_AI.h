#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "CBoss_AI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossSpawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossHealthChanged, float, InValue, float, InMax);

UCLASS()
class P2406_JS_API ACBoss_AI : public ACEnemy_AI
{
	GENERATED_BODY()

public:
	ACBoss_AI();


protected:
	void Damaged() override;

protected:
	void OnHealthPointChanged(float InHealth, float InMaxHealth) override;


protected:
	UPROPERTY(VisibleAnywhere)
	class UCPatternComponent* Pattern;

public:
	FOnBossSpawned OnBossSpawned; 
	FOnBossDamaged OnBossDamage;
	FOnBossHealthChanged OnBossHealthChanged;
};
