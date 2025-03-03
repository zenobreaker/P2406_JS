#include "Notifies/CAnimNotify_Pattern.h"
#include "Global.h"

#include "Components/CPatternComponent.h"


UCAnimNotify_Pattern::UCAnimNotify_Pattern()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Blue;
#endif 
}

FString UCAnimNotify_Pattern::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("Pattern", Type);
}

void UCAnimNotify_Pattern::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(MeshComp->GetOwner());
	CheckNull(pattern);

	switch (Type)
	{
	case EAnimNotify_Flow::Begin:
		pattern->Begin_Pattern();
		break;
	case EAnimNotify_Flow::End:
		pattern->End_Pattern();
		break;
	}
}
