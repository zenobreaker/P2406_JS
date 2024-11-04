#include "Notifies/SubAction/CAnimNotify_SubAction_Hammer.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SubActions/CSubAction_Hammer.h"


FString UCAnimNotify_SubAction_Hammer::GetNotifyName_Implementation() const
{
	return "SubAction_Hammer";
}

void UCAnimNotify_SubAction_Hammer::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

	UCSubAction_Hammer* Hammer = Cast<UCSubAction_Hammer>(weapon->GetSubAction());
	CheckNull(Hammer);

	Hammer->CreateEffectSubActionHammer();
	Hammer->TraceAttackArea();
}
