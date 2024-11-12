#include "Notifies/CAnimNotifyState_Dash.h"
#include "Global.h"
#include "Components/CDashComponent.h"
#include "Components/CMovementComponent.h"

FString UCAnimNotifyState_Dash::GetNotifyName_Implementation() const
{
	return "Dash";
}

void UCAnimNotifyState_Dash::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCDashComponent* dash = CHelpers::GetComponent<UCDashComponent>(MeshComp->GetOwner());
	CheckNull(dash);

	dash->Begin_DashSpeed();
	
}

void UCAnimNotifyState_Dash::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCDashComponent* dash = CHelpers::GetComponent<UCDashComponent>(MeshComp->GetOwner());
	CheckNull(dash);

	dash->End_DashSpeed();
}

