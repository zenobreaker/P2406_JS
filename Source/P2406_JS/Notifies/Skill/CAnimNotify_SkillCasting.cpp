#include "Notifies/Skill/CAnimNotify_SkillCasting.h"
#include "Global.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_SkillCasting::GetNotifyName_Implementation() const
{
	return "SkillCasting";
}

void UCAnimNotify_SkillCasting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner()); 


	UCSkillComponent* skill = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	CheckNull(skill); 

	skill->OnSkillCasting();
}
