#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CPlayerController.generated.h"


UCLASS()
class P2406_JS_API ACPlayerController : public APlayerController
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "SpectorCam");
	TSubclassOf<class ASpectatorCameraPawn> SpectorCameraPawnClass;

private:
	ACPlayerController(); 

protected:
	void SetupInputComponent() override;

public:
	void SwitchToSpectorCamPawn(); 


private: 
	class APawn* OriginalPawn; 
	class APawn* SepctorCamPawn;

	bool bUsingSpectatorCamera = false; 
};
