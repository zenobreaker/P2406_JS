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
	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashSpeed = 1000; 

private:
	DashDirection DashDir;

protected:
	virtual void BeginPlay() override;

public:	
	void OnDash();
	void DashAction();

public:
	void Begin_DashSpeed();
	void End_DashSpeed();
		
private:
	ACharacter* OwnerCharacter;
	class UCMovementComponent* Movement;
	class UCTargetComponent* target;
	class UCWeaponComponent* Weapon;
	class UCStateComponent* State; 
private:
	bool bTargeting = false;
	bool bIsDash = false;
};
