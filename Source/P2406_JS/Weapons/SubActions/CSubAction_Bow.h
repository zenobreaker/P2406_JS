#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "Components/TimelineComponent.h"
#include "CSubAction_Bow.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector(0, 30, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag = false;

	UPROPERTY(EditAnywhere)
	FVector CameraLocation = FVector::OneVector;
};


UCLASS(Blueprintable)
class P2406_JS_API UCSubAction_Bow : public UCSubAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Aiming")
	class UCurveVector* Curve;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	FAimData AimData; 

	UPROPERTY(EditAnywhere, Category = "Aiming")
	float AimingSpeed = 200.0f; 


public: 
	UCSubAction_Bow();

public:
	void Pressed() override; 
	void Released() override; 

public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	void Tick(float InDeltaTime) override; 

private:
	UFUNCTION()
	void OnAiming(FVector Output);

private:
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	FAimData OriginData; 

private:
	FTimeline Timeline;

private:
	float* Bending;
};
