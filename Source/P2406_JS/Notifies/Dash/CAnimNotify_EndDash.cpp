#include "Notifies/Dash/CAnimNotify_EndDash.h"
#include "Global.h"
#include "Components/CDashComponent.h"

FString UCAnimNotify_EndDash::GetNotifyName_Implementation() const
{
	return "EndDash";
}

void UCAnimNotify_EndDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCDashComponent* dash = CHelpers::GetComponent<UCDashComponent>(MeshComp->GetOwner());
	CheckNull(dash);

	dash->DestroyEvadeOverlap();
}
