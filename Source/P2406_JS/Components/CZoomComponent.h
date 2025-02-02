#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CZoomComponent.generated.h"


UCLASS( ClassGroup=(Additional), meta=(BlueprintSpawnableComponent, DisplayName = "ÁÜ ÄÄÆ÷³ÍÆ®"))
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

public:	
	UCZoomComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void SetValue(float InZoomValue); 

private:
	class ACharacter* OwnerCharacter;
	class USpringArmComponent* SpringArm;

private:
	float TargetArmLength;
};
