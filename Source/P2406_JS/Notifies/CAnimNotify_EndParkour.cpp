#include "Notifies/CAnimNotify_EndParkour.h"
#include "Global.h"
#include "Components/CParkourComponent.h"

FString UCAnimNotify_EndParkour::GetNotifyName_Implementation() const
{
	return "EndParkour";
}

void UCAnimNotify_EndParkour::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCParkourComponent* parkour = FHelpers::GetComponent<UCParkourComponent>(MeshComp->GetOwner());
	CheckNull(parkour);

	parkour->End_DoParkour();
}
