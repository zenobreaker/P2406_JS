#include "Characters/SpectatorCameraPawn.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"


ASpectatorCameraPawn::ASpectatorCameraPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	FHelpers::CreateComponent(this, &SpringArm, "SpringArm", RootComponent);
	SpringArm->bUsePawnControlRotation = true;

	FHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);
	AutoPossessPlayer = EAutoReceiveInput::Disabled;

	//FHelpers::CreateComponent(this, &Movement, "Movement");
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
}

void ASpectatorCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpectatorCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpectatorCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &ASpectatorCameraPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASpectatorCameraPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ASpectatorCameraPawn::MoveUp);

	PlayerInputComponent->BindAxis("VerticalLook", this, &ASpectatorCameraPawn::LookUp);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ASpectatorCameraPawn::TurnRight);
}

void ASpectatorCameraPawn::MoveForward(float InValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector(); 

	AddMovementInput(direction, InValue);
}

void ASpectatorCameraPawn::MoveRight(float InValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, InValue);
}

void ASpectatorCameraPawn::MoveUp(float InValue)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetUpVector();

	AddMovementInput(direction, InValue);
}

void ASpectatorCameraPawn::TurnRight(float InValue)
{
	AddControllerYawInput(InValue);
}

void ASpectatorCameraPawn::LookUp(float InValue)
{
	AddControllerPitchInput(InValue);
}

