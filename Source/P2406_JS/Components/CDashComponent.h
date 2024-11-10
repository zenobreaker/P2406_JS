#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDashComponent.generated.h"

UENUM(BlueprintType)
enum class DashDirection : uint8
{
	Forward = 0, Back, Left, Right, MAX = Right,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCDashComponent();

public:
	UPROPERTY(EditAnywhere, Category = "Dash")
	TArray<class UAnimMontage*>  DashMontages;

private:
	DashDirection DashDir;

protected:
	virtual void BeginPlay() override;

public:	
	void OnDash();
	void DashAction();
		
private:
	ACharacter* OwnerCharacter;
	class UCMovementComponent* movement;
	class UCTargetComponent* target;

private:
	bool bTargeting = false;
	bool bIsDash = false;
};
