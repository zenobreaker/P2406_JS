#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Buffs/CBuffStructures.h"
#include "CStatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUpdatedStatusData);

USTRUCT(BlueprintType)
struct FStatusData 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float CriticalRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float CriticalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float BuffedAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float BuffedAttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float BuffedDefense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float BuffedCriticalRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float BuffedCriticalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly);
	float BuffedSpeed;

};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCStatComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Stat")
	float BaseAttack = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float BuffedAttack = 0.f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float BaseDefense = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float BuffedDefense = 0.f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float BaseAttackSpeed = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float BuffedAttackSpeed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float BaseCriticalRate = 0.0f;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float BuffedCriticalRate = .0f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float BaseCriticalDamage = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Stat")
	float BuffedCriticalDamage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Stat")
	float BuffedSpeed = 0.0f;

public:
	UCStatComponent();

public:
	FORCEINLINE const FStatusData& GetStatusData() const { return StatusInfo; }

protected:
	virtual void BeginPlay() override;

public:
	void ApplyBuff(const struct FStatBuff& InStatBuff);
	void ApplyBuff(ECharStatType StatType, float InValue);
	void RemoveBuff(const struct FStatBuff& InStatBuff);
	void RemoveBuff(ECharStatType StatType, float InValue);

	float GetStatValue(ECharStatType StatType) const;

	void ApplyStatusInfo();

	UFUNCTION(BlueprintCallable)
	FStatusData GetStatusInfo();

public:
	FOnUpdatedStatusData OnUpdatedStatusData;

private:
	class ACharacter* OwnerCharacter;
	FStatusData StatusInfo; 
};
