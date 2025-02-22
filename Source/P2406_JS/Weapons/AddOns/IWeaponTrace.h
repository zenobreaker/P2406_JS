#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWeaponTrace.generated.h"

UINTERFACE(MinimalAPI)
class UIWeaponTrace : public UInterface
{
	GENERATED_BODY()
};

class P2406_JS_API IIWeaponTrace
{
	GENERATED_BODY()

public:
    virtual FVector CalculateStartLocation() = 0;
    virtual FVector CalculateEndLocation() = 0;
    virtual FQuat CalculateRotation() = 0;
    virtual FCollisionShape GetCollisionShape() = 0;
    virtual ECollisionChannel GetCollisionChannel() = 0;
};
