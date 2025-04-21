#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpectatorCameraPawn.generated.h"

UCLASS()
class P2406_JS_API ASpectatorCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	ASpectatorCameraPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY(VisibleAnywhere) 
	class UCameraComponent* Camera; 

	UPROPERTY(VisibleAnywhere);
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere);
	class  UFloatingPawnMovement* Movement;

private: 
	void MoveForward(float InValue); 
	void MoveRight(float InValue);
	void MoveUp(float InValue);
	void TurnRight(float InValue); 
	void LookUp(float InValue); 

private:
	class ACharacter* OrigianlPlayer;

};
