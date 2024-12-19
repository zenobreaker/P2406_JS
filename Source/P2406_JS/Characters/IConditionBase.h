#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IConditionBase.generated.h"

UINTERFACE(MinimalAPI)
class UIConditionBase : public UInterface
{
	GENERATED_BODY()
};

class P2406_JS_API IIConditionBase
{
	GENERATED_BODY()

public:
	// ���� ���� �޼���
	virtual void OnConditionAdded() = 0;
	virtual void OnConditionRemoved() = 0;
};

