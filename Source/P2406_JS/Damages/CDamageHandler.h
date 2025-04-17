#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "IDamageState.h"
#include "CDamageHandler.generated.h"


UENUM(BlueprintType)
enum class EDamageAnimType : uint8
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
	float Power = 0.0f;
	class ACharacter* Attacker = nullptr;
	class AActor* Causer = nullptr;

public:
	struct FActionDamageEvent* Event;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCDamageHandler : public UActorComponent
{
	GENERATED_BODY()

private: 
	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<EDamageAnimType, struct FDamageAnimData> DamageMontages;

	UPROPERTY(EditAnywhere, Category = "Ciritical")
	class USoundWave* CriticalSound;

	UPROPERTY(EditAnywhere, Category = "Ciritical")
	class UFXSystemAsset* CriticalEffect; 

	UPROPERTY(EditAnywhere, Category = "Ciritical")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Ciritical")
	FRotator EffectRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Ciritical")
	FVector EffectScale = FVector::OneVector;


public:
	UCDamageHandler();
public:
	void BeginPlay() override;
	void OnComponentCreated() override; 

private: 
	float CalcFinalDamage(const float InDamage, const FHitData& InHitData, class AActor* InAttacker, class AActor* InReceiver);

public:
	void ApplyDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
	void ApplyDamage(struct FDamageData& InDamageData);

public:
	void HandleHitEffect(struct FHitData& InHitData, const struct FDamageData& InDamageData);
	void HandleLaunch(struct FHitData& InHitData, class ACharacter* InAttacker, const bool bIsGuarding = false);
	void PlayDamageMontage(struct FHitData& InHitData); 

protected:
	class ACharacter* OwnerCharacter;

protected:
	TMap<EDamageAnimType, IIDamageState*> DamageStateTable;
	FDamageData DamageData;

private: 
	class UCAirborneComponent* Airborne;
	class UCConditionComponent* Condition;
	class UCHealthPointComponent* HealthPoint;
};
