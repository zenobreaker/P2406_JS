#include "Notifies/CAnimNotify_CameraShake.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "LegacyCameraShake.h"

FString UCAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character);

	APlayerController* controller = character->GetController<APlayerController>();
	CheckNull(controller);

	controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
