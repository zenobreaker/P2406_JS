#include "Notifies/JumpFalling/CAnimNotify_JumpFallingEffect.h"
#include "Global.h"

#include "Components/CWeaponComponent.h"
#include "Weapons/DoActions/CDoAction_JumpAction.h"

UCAnimNotify_JumpFallingEffect::UCAnimNotify_JumpFallingEffect()
{
	
}

FString UCAnimNotify_JumpFallingEffect::GetNotifyName_Implementation() const
{
	return "Falling Effect";
}

void UCAnimNotify_JumpFallingEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);

	UCDoAction* doAction = weapon->GetJumpDoAction();
	CheckNull(doAction);

	UCDoAction_JumpAction* jumpAction = Cast<UCDoAction_JumpAction>(doAction);
	CheckNull(jumpAction);

	jumpAction->Play_DownEffect();
}
