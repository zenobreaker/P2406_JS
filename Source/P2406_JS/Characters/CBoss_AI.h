#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy_AI.h"
#include "BehaviorTree/AddOns/CBossPhaseData.h"
#include "CBoss_AI.generated.h"

UENUM(BlueprintType)
enum class EBossState :uint8
{
	Idle, Rotate, Max
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossSpawned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossHealthChanged, float, InValue, float, InMax);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossPhaseUpdated, int32, InPhase);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossStateChanged, EBossState, InType);


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
	FORCEINLINE bool IsBossIdleState() const { return BossStateType == EBossState::Idle; }
	FORCEINLINE bool IsBossRotateState() const { return BossStateType == EBossState::Rotate; }

public:
	ACBoss_AI();

protected:
	virtual void BeginPlay() override;

protected:
	void Damaged() override;

protected:
	void OnHealthPointChanged(float InHealth, float InMaxHealth) override;

public: 
	void SetBossIdleMode();
	void SetBossRotateMode(); 

public:
	void CheckPhaseTransition();
	void SetNextPhase(int32 InNextPhase);

private:
	UFUNCTION()
	void OnRotated(bool InValue);

protected:
	UPROPERTY(VisibleAnywhere)
	class UCPatternComponent* Pattern;

public:
	FOnBossSpawned OnBossSpawned;
	FOnBossDamaged OnBossDamage;
	FOnBossHealthChanged OnBossHealthChanged;
	FOnBossPhaseUpdated OnBossPhaseUpdated;
	FOnBossStateChanged OnBossStateChanged;
private:
	int32 CurrentPhase;
	float StartTime;
	float CurrentTime;
	EBossState BossStateType; 
};
