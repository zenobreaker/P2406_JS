#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHealthPointComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHealthPointChanged, float, InHealth, float, InMaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCHealthPointComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth = 100;


public:
	FORCEINLINE bool IsDead() const { return Health <= 0.0f; }

	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }

public:	
	UCHealthPointComponent();

protected:
	virtual void BeginPlay() override;


public:
	void Damage(float InAmount);

public:
	FHealthPointChanged OnHealthPointChanged;

private:
	float Health;
		
};
