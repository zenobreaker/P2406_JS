#include "Notifies/CAnimNotify_Move.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCAnimNotify_Move::UCAnimNotify_Move()
{
#if WITH_EDITORONLY_DATA
	NotifyColor = FColor::Blue;
#endif 
}

FString UCAnimNotify_Move::GetNotifyName_Implementation() const
{
	return UCAnimNotify_Structures::GetNotifyName("Move", Type);;
}

void UCAnimNotify_Move::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	CheckNull(character); 

	CheckNull(character->GetCharacterMovement());

	switch (Type)
	{
	case EAnimNotify_Flow::Begin:
		break;
	case EAnimNotify_Flow::End:
		character->GetCharacterMovement()->SetMovementMode(MovementMode);
		break;
	}

	
}
