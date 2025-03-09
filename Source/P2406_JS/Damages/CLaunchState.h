#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IDamageState.h"
#include "CLaunchState.generated.h"

UCLASS()
class P2406_JS_API UCLaunchState 
	: public UObject
	, public IIDamageState
{
	GENERATED_BODY()

public:
	void HandleDamage(UCDamageHandler* InDamageHandler, const FHitData& InHitData);
};
