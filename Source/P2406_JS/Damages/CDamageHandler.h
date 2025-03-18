#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IDamageState.h"
#include "CDamageHandler.generated.h"


UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Normal = 0, Strong, Launch, Begin_Down, Down, Airbone, Max,
};

USTRUCT(Blueprintable)
struct FDamageAnimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere);
	class UAnimMontage* Montage = nullptr;

	UPROPERTY(EditAnywhere);
	float PlayRate = 1.0f; 

public:
	FDamageAnimData() = default;
};

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

public:
	float Power;
	class ACharacter* Attacker;
	class AActor* Causer;

	struct FActionDamageEvent* Event;
};

UCLASS(Blueprintable)
class P2406_JS_API UCDamageHandler : public UObject
{
	GENERATED_BODY()

private: 
	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<EDamageType, struct FDamageAnimData> DamageMontages;

public:
	UCDamageHandler();
public:
	void BeginPlay(class ACharacter* InCharacter);
	void ApplyDamage(struct FDamageData& InDamageData, struct FHitData& InHitData);
	void HandleHitEffect(struct FHitData& InHitData, bool bFirstHit = false);
	void HandleLaunch(struct FHitData& InHitData, class ACharacter* InAttacker, const bool bIsGuarding = false);
	void PlayDamageMontage(struct FHitData& InHitData); 

protected:
	class ACharacter* OwnerCharacter;

protected:
	TMap<EDamageType, IIDamageState*> DamageStateTable; 
	FDamageData DamageData;

private: 
	class UCAirborneComponent* Airborne;
	class UCConditionComponent* Condition;
	class UCHealthPointComponent* HealthPoint;
};
