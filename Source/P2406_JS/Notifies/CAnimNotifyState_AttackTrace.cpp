#include "Notifies/CAnimNotifyState_AttackTrace.h"
#include "Global.h"
#include "Components/CAttackTraceComponent.h"
#include "Weapons/CAttachment.h"

FString UCAnimNotifyState_AttackTrace::GetNotifyName_Implementation() const
{
	return "ATrace";
}

void UCAnimNotifyState_AttackTrace::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCAttackTraceComponent* ATrace = FHelpers::GetComponent<UCAttackTraceComponent>(MeshComp->GetOwner());
	CheckNull(ATrace);

	ATrace->SetBeginTrace();
}

void UCAnimNotifyState_AttackTrace::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCAttackTraceComponent* ATrace = FHelpers::GetComponent<UCAttackTraceComponent>(MeshComp->GetOwner());
	CheckNull(ATrace);

	ATrace->SetEndTrace();
}
