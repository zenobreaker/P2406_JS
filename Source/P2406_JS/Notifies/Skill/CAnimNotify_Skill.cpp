#include "Notifies/Skill/CAnimNotify_Skill.h"
#include "Global.h"
#include "Components/CSkillComponent.h"


UCAnimNotify_Skill::UCAnimNotify_Skill()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Blue;
#endif 
}

FString UCAnimNotify_Skill::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("Skill", Type);
}

void UCAnimNotify_Skill::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = FHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	CheckNull(skill); 

	switch (Type)
	{
		case EAnimNotify_Flow::Begin:
		skill->BeginSkill();
		break;

		case EAnimNotify_Flow::End:
		skill->EndSkill();
		break; 
	}
}
