#include "Weapons/SubActions/CAnimNotifyState_Hammer.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SubActions/CSubAction_Hammer.h"

FString UCAnimNotifyState_Hammer::GetNotifyName_Implementation() const
{
	return "SubAction_Hammer";
}

void UCAnimNotifyState_Hammer::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

}

void UCAnimNotifyState_Hammer::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

}
