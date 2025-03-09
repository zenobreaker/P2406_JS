#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageState.generated.h"

UINTERFACE(MinimalAPI)
class UIDamageState : public UInterface
{
	GENERATED_BODY()
};


class P2406_JS_API IIDamageState
{
	GENERATED_BODY()

public:
	virtual void HandleDamage(class UCDamageHandler* InDamageHandler, const struct FHitData& InHitData) = 0;
};
