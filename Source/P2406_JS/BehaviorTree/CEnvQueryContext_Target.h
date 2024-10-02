#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "CEnvQueryContext_Target.generated.h"

UCLASS()
class P2406_JS_API UCEnvQueryContext_Target : public UEnvQueryContext
{
	GENERATED_BODY()

private:
	void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
