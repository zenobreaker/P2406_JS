#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "CEnvQueryTest_ExcludeCurLocation.generated.h"


UCLASS()
class P2406_JS_API UCEnvQueryTest_ExcludeCurLocation : public UEnvQueryTest
{
	GENERATED_BODY()
	
public:
	UCEnvQueryTest_ExcludeCurLocation();

public:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;

private:
	FVector TargetLocation = FVector(0, 0, 0);
	float MaxDistance = 1000.0f; 
	
};
