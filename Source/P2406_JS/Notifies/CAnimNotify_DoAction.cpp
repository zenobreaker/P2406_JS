#include "Notifies/CAnimNotify_DoAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"


UCAnimNotify_DoAction::UCAnimNotify_DoAction()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Blue;
#endif 
}

FString UCAnimNotify_DoAction::GetNotifyName_Implementation() const
{
    return UCAnimNotify_Structures::GetNotifyName("DoAction", Type);
}

void UCAnimNotify_DoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
		weapon->Begin_DoAction();
		break;

		case EAnimNotify_Flow::End:
		weapon->End_DoAction();
		break;
	}
}
