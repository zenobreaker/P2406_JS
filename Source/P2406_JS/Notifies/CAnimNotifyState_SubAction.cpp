#include "Notifies/CAnimNotifyState_SubAction.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"

FString UCAnimNotifyState_SubAction::GetNotifyName_Implementation() const
{
	return "SubAction";
}

void UCAnimNotifyState_SubAction::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

	weapon->GetSubAction()->Begin_DoSubAction();
}

void UCAnimNotifyState_SubAction::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	weapon->GetDoAction()->End_DoAction();

	CheckNull(weapon->GetSubAction());
	weapon->GetSubAction()->End_DoSubAction();

}
