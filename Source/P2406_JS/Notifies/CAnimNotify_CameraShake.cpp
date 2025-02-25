#include "Notifies/CAnimNotify_CameraShake.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "LegacyCameraShake.h"

#include "Characters/CEnemy_AI.h"

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

	// 하필 캐스팅해보니 ai였다 
	APlayerController* controller = nullptr;
	ACEnemy_AI* ai = Cast<ACEnemy_AI>(MeshComp->GetOwner());
	if (ai != nullptr)
		controller = ai->GetWorld()->GetFirstPlayerController();
	else 
		controller = character->GetController<APlayerController>();
	
	CheckNull(controller);

	controller->PlayerCameraManager->StartCameraShake(CameraShakeClass);
}
