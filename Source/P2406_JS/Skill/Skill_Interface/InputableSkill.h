#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputableSkill.generated.h"

UINTERFACE(MinimalAPI)
class UInputableSkill : public UInterface
{
	GENERATED_BODY()
};


class P2406_JS_API IInputableSkill
{
	GENERATED_BODY()

public:
	virtual void PressKey() = 0; 
	virtual void ReleaseKey() = 0; 
	
	virtual void OnPressedKey() = 0; 
	virtual void OnReleasedKey() = 0;
};
