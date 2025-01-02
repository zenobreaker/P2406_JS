#include "Notifies/CAnimNotify_GuardChance.h"
#include "Global.h"

#include "Components/CGuardComponent.h"

FString UCAnimNotify_GuardChance::GetNotifyName_Implementation() const
{
	return "GuardChance";
}

void UCAnimNotify_GuardChance::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCGuardComponent* guard = FHelpers::GetComponent<UCGuardComponent>(MeshComp->GetOwner());

	CheckNull(guard);

	guard->OnJustGuard();
}
