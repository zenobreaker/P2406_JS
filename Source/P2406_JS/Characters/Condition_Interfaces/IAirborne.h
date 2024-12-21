#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAirborne.generated.h"

UINTERFACE(MinimalAPI)
class UIAirborne : public UInterface
{
	GENERATED_BODY()
};


class P2406_JS_API IIAirborne
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnAirborneConditionActivated() = 0;
	UFUNCTION()
	virtual void OnAirborneConditionDeactivated() = 0;
};
