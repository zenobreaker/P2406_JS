
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IStatable.generated.h"

UINTERFACE(MinimalAPI)
class UIStatable : public UInterface
{
	GENERATED_BODY()
};


class P2406_JS_API IIStatable
{
	GENERATED_BODY()

public:
	virtual void End_Backstep() {}
	virtual void End_Dead() {}
	virtual void End_Damaged() {}
	//virtual void End_Downed() {} 
};
