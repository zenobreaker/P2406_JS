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
	UPROPERTY(EditAnywhere, Category = "Dash")
	float BlurAmount = 1.5f;
	UPROPERTY(EditAnywhere, Category = "Dash")
	class USoundWave* Sound;


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
	void PlaySoundWave();

private:
	ACharacter* OwnerCharacter;
	class UCMovementComponent* Movement;
	class UCTargetComponent* target;
	class UCWeaponComponent* Weapon;
	class UCStateComponent* State; 
	class UCameraComponent* Camera;

private:
	bool bTargeting = false;
	bool bIsDash = false;
	FVector inputVec;
};