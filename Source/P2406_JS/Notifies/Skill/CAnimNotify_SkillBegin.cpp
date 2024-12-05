#include "Notifies/Skill/CAnimNotify_SkillBegin.h"
#include "Global.h"
#include "Components/CSkillComponent.h"


FString UCAnimNotify_SkillBegin::GetNotifyName_Implementation() const
{
	return "SkillBegin";
}

void UCAnimNotify_SkillBegin::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference); 

	CheckNull(MeshComp); 
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	CheckNull(skill);

	skill->BeginSkill();
}
