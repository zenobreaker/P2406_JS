#include "Notifies/Skill/CAnimNotify_SkillCollision.h"
#include "Global.h"

#include "Components/CSkillComponent.h"
#include "Components/CPatternComponent.h"

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

	if (skill != nullptr)
	{
		switch (Type)
		{
		case EAnimNotify_Flow::Begin:
			skill->OnActivated_Collision(CollisionName);
			break;
		case EAnimNotify_Flow::End:
			skill->OnDeactivated_Collision(CollisionName);
			break;
		}
	}
	else
	{
		UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(MeshComp->GetOwner());
		CheckNull(pattern);

		switch (Type)
		{
		case EAnimNotify_Flow::Begin:
			pattern->OnActivated_Collision(CollisionName);
			break;
		case EAnimNotify_Flow::End:
			pattern->OnDeactivated_Collision(CollisionName);
			break;
		}
	}
}
