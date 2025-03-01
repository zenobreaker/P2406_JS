#pragma once

#include "CoreMinimal.h"
#include "Characters/CAIController.h"
#include "CAIController_Boss.generated.h"


UCLASS()
class P2406_JS_API ACAIController_Boss : public ACAIController
{
	GENERATED_BODY()
	

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

public:
	void RotateTowardsTarget(float InDeltaTime, ACharacter* InTarget);
};
