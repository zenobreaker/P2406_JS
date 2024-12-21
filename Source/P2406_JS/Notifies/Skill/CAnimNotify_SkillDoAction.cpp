#include "Notifies/Skill/CAnimNotify_SkillDoAction.h"
#include "Global.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_SkillDoAction::GetNotifyName_Implementation() const
{
	return "SkillDoAction";
}

void UCAnimNotify_SkillDoAction::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = FHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner()); 
	CheckNull(skill); 

	skill->OnSkillDoAction(); 
}
