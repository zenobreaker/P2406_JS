#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "CZoomComponent.generated.h"



USTRUCT()
struct FZoomAimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetArmLength = 100;

	UPROPERTY(EditAnywhere)
	FVector SocketOffset = FVector(30, 100, 10);

	UPROPERTY(EditAnywhere)
	bool bEnableCameraLag = false;

	UPROPERTY(EditAnywhere)
	FVector CameraLocation = FVector::OneVector;
};


UCLASS( ClassGroup=(Additional), meta=(BlueprintSpawnableComponent, DisplayName = "줌 컴포넌트"))
class P2406_JS_API UCZoomComponent : public UActorComponent
{ 
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Settings")
	float Speed = 20.0f;

	UPROPERTY(EditAnywhere, Category = "Settings")
	FVector2D Range = FVector2D(100, 500);

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpSpeed = 5.0f; 

	UPROPERTY(EditAnywhere, Category = "Aiming")
	class UCurveVector* Curve; 

	UPROPERTY(EditAnywhere, Category = "Aiming")
	FZoomAimData ZoomAimData;

	UPROPERTY(EditAnywhere, Category = "Aiming")
	float ZoomAimingSpeed = 1000.0f;

public:
	FORCEINLINE void SetZoomOn() { bZoomable = true; }
	FORCEINLINE void SetZoomOff() { bZoomable = false; }
	FORCEINLINE bool GetGrapplingZoomMode() { return bGrapplingAiming; }

public:	
	UCZoomComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetValue(float InZoomValue); 

	void Pressed();
	void Released(); 

private:
	UFUNCTION()
	void OnZoomAiming(FVector Output);

	void SetOriginCameraData();


private:
	class ACharacter* OwnerCharacter;
	class USpringArmComponent* SpringArm;
	class UCameraComponent* Camera;

private:
	float TargetArmLength;
	FTimeline Timeline;

private: 
	bool bZoomable = true; 
	bool bGrapplingAiming = false; 
	FZoomAimData OriginData;

	FVector OriginVector;
	FVector CameraLocation;// 입력되었을 때 방향 
};
