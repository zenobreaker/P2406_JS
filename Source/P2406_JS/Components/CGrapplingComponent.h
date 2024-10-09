#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGrapplingComponent.generated.h"


UCLASS()
class P2406_JS_API UCGrapplingComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	//UPROPERTY(EditAnywhere, Category = "Grappling")
	//class UCableComponent* GrappleCable;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float PullSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	UAnimMontage* GrapplingMontage;

public:
	FORCEINLINE bool GetGrappling() { return bIsGrappling; }

private:
	UCGrapplingComponent();

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void OnGrapple();
	virtual void PullTowardsTarget();
	void OnGrappling_Pressed();
	void OnGrappling_Released();
	void ResetGrapple();
	void InterruptGrapple();

public:
	void Begin_DoGrappling();
	void End_DoGrappling();

private:
	void Grapple_1(float InDetaTime);
	void Grapple_2(float InDetaTime);


private:
	class ACharacter* OwnerCharacter;

private:
	FVector TargetLocation;

	float MaxLineDistance = 1600.0f;
	float DistanceThreshold = 100.0f;
	bool bIsGrappling = false;
	bool bGrappleEnd = false;
	//FVector GrapPoint;

};
