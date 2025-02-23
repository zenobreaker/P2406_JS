#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CAirborneComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCAirborneComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetIsAirborne() { return bIsAirborne; }

public:	
	UCAirborneComponent();

protected:
	virtual void BeginPlay() override;

public:	
	

public:
	//void LaunchIntoAir(float LaunchPower, AActor* InCauser = nullptr);
	//GetAddedAirValue
	float Calc_AirborenValue(float LaunchPower, AActor* InCauser = nullptr);
	
	UFUNCTION()
	void Landed();
	
	UFUNCTION()
	void OnCharacterEndDamaged();
	
private:
	bool bIsAirborne;
	float OriginGravity;

private: 
	ACharacter* OwnerCharacter;
	AActor* Causer;
};
