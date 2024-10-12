#include "Notifies/CAnimNotify_BeginGrappling.h"
#include "Global.h"
#include "Components/CGrapplingComponent.h"

FString UCAnimNotify_BeginGrappling::GetNotifyName_Implementation() const
{
	return "Begin_Grappling";
}

void UCAnimNotify_BeginGrappling::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCGrapplingComponent* grappling = CHelpers::GetComponent<UCGrapplingComponent>(MeshComp->GetOwner());
	CheckNull(grappling);

	grappling->Begin_DoGrappling();
}
