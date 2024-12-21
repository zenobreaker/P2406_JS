#include "Notifies/Skill/CAnimNotify_SkillEnd.h"
#include "Global.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_SkillEnd::GetNotifyName_Implementation() const
{
	return "SkillEnd";
}

void UCAnimNotify_SkillEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = FHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	CheckNull(skill);

	skill->EndSkill();
}
