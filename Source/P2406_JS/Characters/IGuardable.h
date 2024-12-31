#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IGuardable.generated.h"

UINTERFACE(MinimalAPI)
class UIGuardable : public UInterface
{
	GENERATED_BODY()
};

class P2406_JS_API IIGuardable
{
	GENERATED_BODY()

public:
	// 가드 가능한지 체크하는 메서드
	virtual bool CanGuard() const = 0;

	virtual bool GetGuarding() const = 0; 

	// 가드를 시작하는 메서드
	virtual void StartGuard() = 0;

	// 가드를 해제하는 메서드
	virtual void StopGuard() = 0;

};
