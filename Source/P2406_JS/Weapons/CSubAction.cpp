#include "Weapons/CSubAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CWeaponComponent.h"


UCSubAction::UCSubAction()
{

}

void UCSubAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;

	State = CHelpers::GetComponent<UCStateComponent>(Owner);
	Movement = CHelpers::GetComponent<UCMovementComponent>(Owner);
	Weapon = CHelpers::GetComponent<UCWeaponComponent>(Owner);
}

void UCSubAction::Pressed()
{
	bInAction = true; 
}

void UCSubAction::Released()
{
	bInAction = false; 
}


