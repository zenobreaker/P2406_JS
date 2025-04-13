#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "Characters/IDamagable.h"
#include "CAttachment_Destructible.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDestroyedObject);

UCLASS()
class P2406_JS_API ACAttachment_Destructible
	: public ACAttachment
	, public IIDamagable
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	class UCHealthPointComponent* HealthPoint; 

	UPROPERTY(VisibleAnywhere)
	class UCDamageHandler* DamageHandler; 

public: 
	ACAttachment_Destructible(); 

public:
	FORCEINLINE void SetDamagable(bool InValue) { bDamagable = InValue; }

public:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	FDestroyedObject OnDestroyedObject; 

public:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	
private:
	bool bDamagable = true; 
};
