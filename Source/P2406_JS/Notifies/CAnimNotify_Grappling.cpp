#include "Notifies/CAnimNotify_Grappling.h"
#include "Global.h"
#include "Components/CGrapplingComponent.h"

UCAnimNotify_Grappling::UCAnimNotify_Grappling()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
#endif 
}

FString UCAnimNotify_Grappling::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("Grappling", Type);
}

void UCAnimNotify_Grappling::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCGrapplingComponent* Grappling = FHelpers::GetComponent<UCGrapplingComponent>(MeshComp->GetOwner());
	CheckNull(Grappling);

	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			Grappling->Begin_DoGrappling(); 
			break;
		case EAnimNotify_Flow::Middle:
			Grappling->Check_GrapleMode();
			break;
		case EAnimNotify_Flow::End:
			Grappling->End_DoGrappling();
			break;
	}

}
