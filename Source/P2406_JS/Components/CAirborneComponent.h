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
	void LaunchIntoAir(float LaunchPower, AActor* InCauser = nullptr);
	
	void Landed(const FHitResult& Hit);
	
private:
	bool bIsAirborne;

	ACharacter* OwnerCharacter;
	UCharacterMovementComponent* movement;
	AActor* Causer;
};
