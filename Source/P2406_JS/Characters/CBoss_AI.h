#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "BehaviorTree/AddOns/CBossPhaseData.h"
#include "CBoss_AI.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossSpawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossHealthChanged, float, InValue, float, InMax);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseUpdated, int32, InPhase);

UCLASS()
class P2406_JS_API ACBoss_AI : public ACEnemy_AI
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Boss")
	bool bSuperArmor = true;

	UPROPERTY(EditAnywhere, Category = "Boss")
	UCBossPhaseData* BossPhaseData;

public:
	FORCEINLINE int32 GetCurrentPhase() const { return CurrentPhase; }

public:
	ACBoss_AI();

protected:
	virtual void BeginPlay() override;

protected:
	void Damaged() override;

protected:
	void OnHealthPointChanged(float InHealth, float InMaxHealth) override;

public:
	void CheckPhaseTransition();
	void SetNextPhase(int32 InNextPhase);

protected:
	UPROPERTY(VisibleAnywhere)
	class UCPatternComponent* Pattern;

public:
	FOnBossSpawned OnBossSpawned;
	FOnBossDamaged OnBossDamage;
	FOnBossHealthChanged OnBossHealthChanged;
	FOnBossPhaseUpdated OnBossPhaseUpdated;

private:
	int32 CurrentPhase;
	float StartTime;
	float CurrentTime;
};
