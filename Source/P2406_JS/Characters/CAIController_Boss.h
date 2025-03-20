#pragma once

#include "CoreMinimal.h"
#include "Characters/CAIController.h"
#include "CAIController_Boss.generated.h"


UCLASS()
class P2406_JS_API ACAIController_Boss : public ACAIController
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere, Category ="Rotation")
	FVector2D RotateAngleRange= FVector2D(0, 90.0f);

	UPROPERTY(EditAnywhere, Category ="Rotation")
	FVector2D RotateSpeedRange  = FVector2D(15.0f, 50.0f);

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	void RotateTowardsTarget(float InDeltaTime, ACharacter* InTarget);
};
