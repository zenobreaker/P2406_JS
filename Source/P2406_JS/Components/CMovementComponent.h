#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType :uint8
{
	Walk = 0, Run, Sprint, Max,
};


UCLASS()
class P2406_JS_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float HorizontalLook = 45;
	
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float VerticalLook = 45;

private:
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(int32)ESpeedType::Max] = { 200, 400, 600 };
		
public:
	FORCEINLINE bool CanMove() const { return bCanMove; }
	FORCEINLINE const bool* GetCanMovePtr() { return &bCanMove; }

	FORCEINLINE bool GetFixedCamera() const { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }

	FORCEINLINE float GetWalkSpeed() const { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() const { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() const { return Speed[(int32)ESpeedType::Sprint]; }

	FORCEINLINE const bool* GetCanControlRotaionPtr() { return &bCanControlRotaion; }
public:
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void OnSprint();
	void OnRun();
	void OnWalk();

public:
	void SetSpeed(ESpeedType InType);

public:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

public:
	void EnableControlRotation();
	void DisableControlRotation();

public:
	void Move();
	void Stop();



private:
	class ACharacter* OwnerCharacter;
	FVector InputDirection;

private:
	bool bCanMove = true;
	bool bCanControlRotaion;
	bool bFixedCamera;

};
