#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class P2406_JS_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ACPlayer();

	
private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

private:
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent*Movement;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
