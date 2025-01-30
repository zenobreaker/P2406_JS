#pragma once

#include "CoreMinimal.h"
#include "Characters/CBaseCharacter.h"
#include "CGuardStructures.h"
#include "UObject/NoExportTypes.h"
#include "CDoGuard.generated.h"


UCLASS()
class P2406_JS_API UCDoGuard : public UObject
{
	GENERATED_BODY()
	
public:
	FORCEINLINE bool GetCanGuard() const { return bCanGuard; }
	FORCEINLINE void SetCanGuard(const bool InValue) { bCanGuard = InValue; }
	
	FORCEINLINE bool GetGuarding() const { return bGuarding; }

	FORCEINLINE bool GetCountering() const { return bCountering; }

	FORCEINLINE bool GetJustTime() const { return bJustTime; }

	FORCEINLINE float GetGuardHP() const { return GuardHP; }
	FORCEINLINE float GetMaxGuardHP() const { return GuardData.MaxGuardHP; }

	FORCEINLINE float GetGuardAngle() const { return GuardData.GuardAngle; }

	FORCEINLINE bool GetCanMove() const { return GuardData.bCanMove; }

public: 
	UCDoGuard(); 

public:
	void BeginPlay
	(
		class ACharacter* InOwnerCharacter,
		const FGuardData& InGuardData,
		const FGuardHitData& InGuardHitData,
		const FCounterData& InCounterData
	);

	void Tick(float DeltaTime);

public:
	void Begin_Guard(); 

	void End_Guard();

	bool CheckBlocking(FVector InForward, FVector InAttacker);

	void Damage_Guard();

	void CalcGuardHP(const float InDeltaTime = 0.0f);

	void CalcGuardCooldown(const float InDeltaTime);

	void OnJustGuard();

	// 카운터를 바로하는 가드용 
	void StartCounterGuard();

	void StopCounterGuard();

private:
	class ACharacter* OwnerCharacter;

	FGuardData GuardData;
	FGuardHitData GuardHitData;
	FCounterData CounterData;

private:
	bool bCanGuard = true; 
	bool bGuarding = false;

	float GuardHP = 0.0f;
	float GuardCooldown = 0.0f;

	float CounterTime = 0.0f;
	float CounterRecoveryTime = 0.0f;
	bool bCountering;

	bool bJustTime = false; 

};
