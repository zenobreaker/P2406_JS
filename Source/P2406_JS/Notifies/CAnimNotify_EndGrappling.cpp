#include "Notifies/CAnimNotify_EndGrappling.h"
#include "Global.h"
#include "Components/CGrapplingComponent.h"

FString UCAnimNotify_EndGrappling::GetNotifyName_Implementation() const
{
	return "EndGrappling";
}

void UCAnimNotify_EndGrappling::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCGrapplingComponent* grappling = CHelpers::GetComponent<UCGrapplingComponent>(MeshComp->GetOwner());
	CheckNull(grappling);

	grappling->End_DoGrappling();

}
