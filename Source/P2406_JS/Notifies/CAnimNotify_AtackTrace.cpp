#include "Notifies/CAnimNotify_AtackTrace.h"
#include "Global.h"
#include "Components/CAttackTraceComponent.h"


UCAnimNotify_AtackTrace::UCAnimNotify_AtackTrace()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Green;
#endif 
}

FString UCAnimNotify_AtackTrace::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("ATrace", Type);
}

void UCAnimNotify_AtackTrace::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCAttackTraceComponent* ATrace = FHelpers::GetComponent<UCAttackTraceComponent>(MeshComp->GetOwner());
	CheckNull(ATrace);


	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
			ATrace->SetBeginTrace();
			break;

		case EAnimNotify_Flow::End:
			ATrace->SetEndTrace();
			break;
	}
}
