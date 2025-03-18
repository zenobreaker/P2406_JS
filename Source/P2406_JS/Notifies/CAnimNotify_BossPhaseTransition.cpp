#include "Notifies/CAnimNotify_BossPhaseTransition.h"
#include "Global.h"

#include "Characters/CBoss_AI.h"


UCAnimNotify_BossPhaseTransition::UCAnimNotify_BossPhaseTransition()
{
#if WITH_EDITORONLY_DATA
	FLinearColor color = FLinearColor(0.3f, 1.0f, 0.5f);
	NotifyColor = color.ToFColor(true);
#endif 
}

FString UCAnimNotify_BossPhaseTransition::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("PhaseTransition", Type);
}

void UCAnimNotify_BossPhaseTransition::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBoss_AI* boss = Cast<ACBoss_AI>(MeshComp->GetOwner());
	CheckNull(boss);

	switch (Type)
	{
	case EAnimNotify_Flow::Begin:
		boss->StartOpeningPattern();
		break; 

	case EAnimNotify_Flow::End:
		boss->EndOpeningPattern();
		break;
	}

}
