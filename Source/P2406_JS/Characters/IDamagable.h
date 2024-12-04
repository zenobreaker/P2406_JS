#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamagable.generated.h"

UINTERFACE(MinimalAPI)
class UIDamagable : public UInterface
{
	GENERATED_BODY()
};


class P2406_JS_API IIDamagable
{
	GENERATED_BODY()

public:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) { return 0.0f; };

};
