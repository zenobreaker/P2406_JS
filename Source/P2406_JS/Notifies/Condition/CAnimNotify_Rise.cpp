#include "Notifies/Condition/CAnimNotify_Rise.h"
#include "Global.h"
#include "Components/CConditionComponent.h"

FString UCAnimNotify_Rise::GetNotifyName_Implementation() const
{
	return "Rise";
}

void UCAnimNotify_Rise::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


}

