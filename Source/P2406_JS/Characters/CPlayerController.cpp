#include "Characters/CPlayerController.h"
#include "Global.h"

#include "Characters/SpectatorCameraPawn.h"


ACPlayerController::ACPlayerController()
{
	SpectorCameraPawnClass = ASpectatorCameraPawn::StaticClass();
}

void ACPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("SwitchCamera", EInputEvent::IE_Pressed, this, &ACPlayerController::SwitchToSpectorCamPawn);
}

void ACPlayerController::SwitchToSpectorCamPawn()
{
	if (SepctorCamPawn == nullptr)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		Params.Instigator = GetPawn();

		FVector pawnLocation = GetPawn() != nullptr ? GetPawn()->GetActorLocation() : FVector::ZeroVector;
		FRotator pawnRotator = GetPawn() != nullptr ? GetPawn()->GetActorRotation() : FRotator::ZeroRotator;

		SepctorCamPawn = GetWorld()->SpawnActor<ASpectatorCameraPawn>(
			SpectorCameraPawnClass, pawnLocation, pawnRotator, Params);
	}

	if (bUsingSpectatorCamera)
	{
		Possess(OriginalPawn);
		bUsingSpectatorCamera = false; 
	}
	else
	{
		if (SepctorCamPawn)
		{
			OriginalPawn = GetPawn();
			Possess(SepctorCamPawn);
			bUsingSpectatorCamera = true;
		}
	}
}
