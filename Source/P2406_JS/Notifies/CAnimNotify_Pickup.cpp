#include "Notifies/CAnimNotify_Pickup.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"


#include "Characters/CBoss_AI.h"
#include "Characters/CAIController.h"
#include "Weapons/CAttachment.h"
#include "Weapons/Attachments/CAttachment_Destructible.h"


UCAnimNotify_Pickup::UCAnimNotify_Pickup()
{
#if WITH_EDITORONLY_DATA
	FLinearColor color = FLinearColor(0.3f, 1.0f, 0.5f);
	NotifyColor = color.ToFColor(true);
#endif 
}

FString UCAnimNotify_Pickup::GetNotifyName_Implementation() const
{
	return  UCAnimNotify_Structures::GetNotifyName("Pickup", Type);
}

void UCAnimNotify_Pickup::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ACBoss_AI* boss = Cast<ACBoss_AI>(MeshComp->GetOwner());
	if (!!boss)
	{
		ACAIController* aiController = Cast<ACAIController>(boss->GetController());
		CheckNull(aiController);
		UBlackboardComponent* blackboard = aiController->GetBlackboardComponent(); 
		CheckNull(blackboard); 

		ACAttachment* pickup = Cast<ACAttachment>(blackboard->GetValueAsObject("PickupObject"));
		CheckNull(pickup);

		FHelpers::AttachTo(pickup, boss->GetMesh(), "Right_Trunk", EAttachmentRule::SnapToTarget);
		ACAttachment_Destructible* destructible = Cast<ACAttachment_Destructible>(pickup);
		CheckNull(destructible); 
		
		destructible->SetDamagable(false);
	}
}
