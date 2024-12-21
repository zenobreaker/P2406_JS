#include "Notifies/SubAction/CAnimNotify_SubAction_Sword.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SubActions/CSubAction_Sword.h"

FString UCAnimNotify_SubAction_Sword::GetNotifyName_Implementation() const
{
	return "SubAction_Sword";
}

void UCAnimNotify_SubAction_Sword::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

	UCSubAction_Sword* sword = Cast<UCSubAction_Sword>(weapon->GetSubAction());
	CheckNull(sword); 

	sword->TraceAttackArea();
}
