#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "Characters/IStatable.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());


	IIStatable* state = Cast<IIStatable>(MeshComp->GetOwner());
	CheckNull(state);

	switch (StateType)
	{
		case EStateType::Evade: state->End_Backstep(); break;
		case EStateType::Dead: state->End_Dead(); break;
		case EStateType::Damaged: state->End_Damaged(); break;
	}
}
