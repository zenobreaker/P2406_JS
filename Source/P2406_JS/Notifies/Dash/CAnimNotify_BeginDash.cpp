#include "Notifies/Dash/CAnimNotify_BeginDash.h"
#include "Global.h"
#include "Components/CDashComponent.h"

FString UCAnimNotify_BeginDash::GetNotifyName_Implementation() const
{
	return "BeginDash";
}

void UCAnimNotify_BeginDash::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCDashComponent* dash = FHelpers::GetComponent<UCDashComponent>(MeshComp->GetOwner());
	CheckNull(dash);

	dash->CreateEvadeOverlap(MeshComp->GetOwner()->GetActorLocation());
}
