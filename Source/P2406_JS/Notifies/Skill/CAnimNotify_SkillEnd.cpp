#include "Notifies/Skill/CAnimNotify_SkillEnd.h"
#include "Global.h"

FString UCAnimNotify_SkillEnd::GetNotifyName_Implementation() const
{
	return "SkillEnd";
}

void UCAnimNotify_SkillEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

}
