#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IComboable.generated.h"

UINTERFACE(MinimalAPI)
class UIComboable : public UInterface
{
	GENERATED_BODY()
};


class P2406_JS_API IIComboable
{
	GENERATED_BODY()

public:
	virtual void EnableCombo() { }
	virtual void DisableCombo() { }
};
