#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDownable.generated.h"


UINTERFACE(MinimalAPI)
class UIDownable : public UInterface
{
	GENERATED_BODY()
};

class P2406_JS_API IIDownable
{
	GENERATED_BODY()

public:
	UFUNCTION()
	virtual void OnDownConditionActivated() {};
	UFUNCTION()
	virtual void OnDownConditionDeactivated() {};
};
