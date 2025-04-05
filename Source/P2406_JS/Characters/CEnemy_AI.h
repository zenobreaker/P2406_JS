#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "Characters/IGuardable.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class P2406_JS_API ACEnemy_AI 
	: public ACEnemy
	, public IIGuardable
{
	GENERATED_BODY()

private:
	static int32 GlobalID;
	int32 AIID;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	TSoftObjectPtr<class ACPatrolPath> PatrolPath;

	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxGuardHealth = 50.0f; 

private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon; 


	UPROPERTY(VisibleAnywhere)
	class UCGuardComponent* Guard; 

	UPROPERTY(VisibleAnywhere)
	class UCAttackTraceComponent* ATrace; 

protected:
	UPROPERTY(VisibleAnywhere)
	class UCAIBehaviorComponent* Behavior;


public:
	FORCEINLINE int32 GetAIID() const { return AIID; }
	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE TSoftObjectPtr<class ACPatrolPath> GetPatrolPath() { return PatrolPath; }
public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override; 
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	void Launch(const FHitData& InHitData, const bool bIsGuarding = false) override;

	virtual void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType) override;

protected:
	void Damaged() override; 

protected:
	void Dead() override;


public:
	void End_Damaged() override; 


protected:
	void SetRegisterToBattleManager();

public:
	virtual bool HasGuard() const override;

	// IIGuardable을(를) 통해 상속됨
	bool CanGuard() const override;

	bool GetGuarding() const override;

	void StartGuard() override;

	void StopGuard() override;

	bool CheckBlocking(FDamageData& InDamagedata);

private:
	class UCUserWidget_Enemy* EnemyWidget; 
};
