#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ChargableSkill.generated.h"

UINTERFACE(MinimalAPI)
class UChargableSkill : public UInterface
{
	GENERATED_BODY()
};


class P2406_JS_API IChargableSkill
{
	GENERATED_BODY()


public:
	virtual void StartCharing() = 0; 
	virtual void ReleaseCharge() = 0;
};
