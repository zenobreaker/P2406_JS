#include "Notifies/Skill/CAnimNotify_EndCasting.h"
#include "Global.h"
#include "Components/CSkillComponent.h"

FString UCAnimNotify_EndCasting::GetNotifyName_Implementation() const
{
	return "EndCasting";
}

void UCAnimNotify_EndCasting::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	UCSkillComponent* skill = CHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());

	CheckNull(skill);

	skill->OffSkillCasting();
}
