#include "Components/CDashComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CEquipment.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CWeaponComponent.h"

UCDashComponent::UCDashComponent()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

}


void UCDashComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!!OwnerCharacter)
	{
		Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
		target = CHelpers::GetComponent<UCTargetComponent>(OwnerCharacter);
		Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
		State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	}
}



void UCDashComponent::OnDash()
{
	CheckNull(Movement);

	DashAction();
}

void UCDashComponent::DashAction()
{
	CheckNull(Movement);
	//CheckNull(target);

	FVector* input = Movement->GetInputDirection();
	if (input == nullptr )
		return; 

	// 전방
	OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Forward]);
	
	//// 후방 
	//else if(input->X < 0)
	//{
	//	OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Back]);
	//}
}

void UCDashComponent::Begin_DashSpeed()
{
	FVector dashDir = OwnerCharacter->GetActorForwardVector();
	OwnerCharacter->LaunchCharacter(dashDir * DashSpeed, true, true);

	CheckNull(Weapon);
	CheckNull(Weapon->GetEquipment());

	if (Weapon->GetEquipment()->GetBeginEquip() == true)
		Weapon->GetEquipment()->End_Equip();
}

void UCDashComponent::End_DashSpeed()
{
	//Movement->SetSpeed(ESpeedType::Run);
	CheckNull(Weapon);
	

	CheckNull(State); 

	if (State->IsEquipMode() == true)
	{
		if (Weapon->GetEquipment()->GetBeginEquip() == false)
			Weapon->GetEquipment()->Begin_Equip();
	}

	if (State->IsIdleMode() == false)
	{
		State->SetIdleMode();
	}


}

