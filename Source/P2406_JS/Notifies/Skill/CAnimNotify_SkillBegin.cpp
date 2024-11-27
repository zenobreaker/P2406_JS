#include "Notifies/Skill/CAnimNotify_SkillBegin.h"
#include "Global.h"


FString UCAnimNotify_SkillBegin::GetNotifyName_Implementation() const
{
	return "SkillBegin";
}

void UCAnimNotify_SkillBegin::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

}
