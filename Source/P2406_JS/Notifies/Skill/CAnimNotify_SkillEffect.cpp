#include "Notifies/Skill/CAnimNotify_SkillEffect.h"
#include "Global.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_SkillEffect::GetNotifyName_Implementation() const
{
	return "SkillEffect";
}

void UCAnimNotify_SkillEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = FHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	CheckNull(skill);
	skill->CreateSkillEffect();

}
