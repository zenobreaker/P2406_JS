#include "Notifies/CAnimNotify_EndSubAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"

FString UCAnimNotify_EndSubAction::GetNotifyName_Implementation() const
{
	return "End_SubAction";
}

void UCAnimNotify_EndSubAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	weapon->GetDoAction()->End_DoAction();

	CheckNull(weapon->GetSubAction());
	weapon->GetSubAction()->End_DoSubAction();

}
