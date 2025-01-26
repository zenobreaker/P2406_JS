#include "Weapons/CDoAction.h"
#include "Global.h"
#include "CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"


UCDoAction::UCDoAction()
{

}

void UCDoAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	HitDatas = InHitDatas;
}

void UCDoAction::DoAction()
{
	bInAction = true;

	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;

	bInAction = false;
	State->SetIdleMode();

	DYNAMIC_EVENT_CALL(OnEndedDoAction);

	Movement->Move();
}
