#include "Notifies/Condition/CAnimNotify_Down.h"
#include "Global.h"
#include "Components/CConditionComponent.h"

FString UCAnimNotify_Down::GetNotifyName_Implementation() const
{
	return "Down";
}

void UCAnimNotify_Down::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	
}
