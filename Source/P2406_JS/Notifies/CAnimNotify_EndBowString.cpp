#include "Notifies/CAnimNotify_EndBowString.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/DoActions/CDoAction_Bow.h"

FString UCAnimNotify_EndBowString::GetNotifyName_Implementation() const
{
	return "EndBowString";
}

void UCAnimNotify_EndBowString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	UCDoAction_Bow* bow = Cast<UCDoAction_Bow>(weapon);
	CheckNull(bow);

	bow->End_BowString();
}
