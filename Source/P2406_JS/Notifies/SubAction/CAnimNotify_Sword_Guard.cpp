#include "Notifies/SubAction/CAnimNotify_Sword_Guard.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SubActions/CSubAction_Sword_Guard.h"


FString UCAnimNotify_Sword_Guard::GetNotifyName_Implementation() const
{
	return "Guard";
}

void UCAnimNotify_Sword_Guard::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

	UCSubAction_Sword_Guard* guard = Cast<UCSubAction_Sword_Guard>(weapon->GetSubAction());

	CheckNull(guard);
}
