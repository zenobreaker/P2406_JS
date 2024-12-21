#include "Notifies/SubAction/CAnimNotify_Hammer_Charging.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SubActions/CSubAction_Hammer.h"

FString UCAnimNotify_Hammer_Charging::GetNotifyName_Implementation() const
{
	return "Charging";
}

void UCAnimNotify_Hammer_Charging::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

	UCSubAction_Hammer* Hammer = Cast<UCSubAction_Hammer>(weapon->GetSubAction());
	CheckNull(Hammer);

	Hammer->CreateEffect_Charging();
}
