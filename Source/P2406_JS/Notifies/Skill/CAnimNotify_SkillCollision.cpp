#include "Notifies/Skill/CAnimNotify_SkillCollision.h"
#include "Global.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_SkillCollision::GetNotifyName_Implementation() const
{
	return "Skill_Collision";
}

void UCAnimNotify_SkillCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	CheckNull(skill);

	skill->CreateSkillCollision();
}
