#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CGrapplingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCGrapplingComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Grappling")
	class UCableComponent* GrappleCable;

private:
	UCGrapplingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void OnGrapple();
	virtual void PullTowardsTarget();
	void ResetGrapple();

private:
	class ACharacter* OwnerCharacter; 

private:
	FVector TargetLocation;
	float PullSpeed; 
	float MaxLineDistance = 1600.0f; 
	bool isGrappling = false; 
	FVector GrapPoint;
		
};
