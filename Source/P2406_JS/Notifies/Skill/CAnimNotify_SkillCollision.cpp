#include "Notifies/Skill/CAnimNotify_SkillCollision.h"
#include "Global.h"
#include "Components/CSkillComponent.h"

UCAnimNotify_SkillCollision::UCAnimNotify_SkillCollision()
{
#if WITH_EDITORONLY_DATA
	FLinearColor color = FLinearColor(0.3f, 1.0f, 0.5f);
	NotifyColor = color.ToFColor(true);
#endif 
}

FString UCAnimNotify_SkillCollision::GetNotifyName_Implementation() const
{
	return  UCAnimNotify_Structures::GetNotifyName("Skill_Collision", Type);
}

void UCAnimNotify_SkillCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = FHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	CheckNull(skill);

	switch (Type)
	{
	case EAnimNotify_Flow::Begin:
		skill->OnActivated_Collision();
		break; 
	case EAnimNotify_Flow::End:
		skill->OnDeactivated_Collision();
		break;
	}
}
