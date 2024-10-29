#include "Notifies/SubAction/CAnimNotifyState_SubAction_Sword.h"
#include "Global.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/SubActions/CSubAction_Sword.h"

FString UCAnimNotifyState_SubAction_Sword::GetNotifyName_Implementation() const
{
	return "SubAction_Sword";
}

void UCAnimNotifyState_SubAction_Sword::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

	UCSubAction_Sword* sword = Cast<UCSubAction_Sword>(weapon->GetSubAction());
	CheckNull(sword);

	sword->Begin_DoSubAction();
	sword->TraceAttackArea();
	sword->CreateAttackSlash();
}

void UCAnimNotifyState_SubAction_Sword::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCWeaponComponent* weapon = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetSubAction());

	UCSubAction_Sword* sword = Cast<UCSubAction_Sword>(weapon->GetSubAction());
	CheckNull(sword);

	sword->End_DoSubAction();
	sword->OnAttachmentEndCollision();
}
