#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Buffs/CBuffStructures.h"
#include "CStatComponent.generated.h"


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

protected:
	virtual void BeginPlay() override;

public:
	void ApplyBuff(const struct FStatBuff& InStatBuff);
	void ApplyBuff(ECharStatType StatType, float InValue);
	void RemoveBuff(const struct FStatBuff& InStatBuff);
	void RemoveBuff(ECharStatType StatType, float InValue);

	float GetStatValue(ECharStatType StatType) const;

private:
	class ACharacter* OwnerCharacter;


};
