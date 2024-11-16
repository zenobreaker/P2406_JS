#include "Components/CDashComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CEquipment.h"
#include "Camera/CameraComponent.h"
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
		Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
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
	if (input == nullptr)
		return;
	inputVec = *input;

	// 자유 카메라 모드 일 때 
	if (Weapon->GetEquipment() == nullptr 
		|| Weapon->GetEquipment()->GetControlRotation() == false)
	{
		// 전방
		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Forward]);
	}
	// 고정 회전 모드 일때
	else
	{
		DashDirection dir = DashDirection::Forward;
		// 전방
		if (input->X > 0)
		{
			if (input->Y > 0)
				dir = DashDirection::Right;
			else if(input->Y < 0)
				dir = DashDirection::Left;
		}
		// 후방 
		else
		{
			dir = DashDirection::Back;
			if (input->Y > 0)
				dir = DashDirection::Right;
			else if (input->Y < 0)
				dir = DashDirection::Left;
		}

		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)dir]);
	}
	//// 후방 
	//else if(input->X < 0)
	//{
	//	OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Back]);
	//}
}

void UCDashComponent::Begin_DashSpeed()
{
	FVector dashDir = OwnerCharacter->GetActorForwardVector();
	
	if (Weapon->GetEquipment() != nullptr
		&& Weapon->GetEquipment()->GetControlRotation() == true)
	{
		dashDir = inputVec.X * OwnerCharacter->GetActorForwardVector() +
			inputVec.Y * OwnerCharacter->GetActorRightVector();
	}

	OwnerCharacter->LaunchCharacter(dashDir * DashSpeed, true, true);

	if (!!Camera)
	{
		Camera->PostProcessSettings.bOverride_MotionBlurAmount = true;
		Camera->PostProcessSettings.MotionBlurAmount = BlurAmount;
	}

	//Sound
	PlaySoundWave();

	CheckNull(Weapon);
	CheckNull(Weapon->GetEquipment());

	if (Weapon->GetEquipment()->GetBeginEquip() == true)
		Weapon->GetEquipment()->End_Equip();
}

void UCDashComponent::End_DashSpeed()
{
	//Movement->SetSpeed(ESpeedType::Run);
	CheckNull(Weapon);

	if (!!Camera)
	{
		Camera->PostProcessSettings.bOverride_MotionBlurAmount = false;
		Camera->PostProcessSettings.MotionBlurAmount = 0.0f;
	}


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

void UCDashComponent::PlaySoundWave()
{
	CheckNull(Sound);
	CheckNull(OwnerCharacter);

	UWorld* world = OwnerCharacter->GetWorld();
	FVector location = OwnerCharacter->GetActorLocation();


	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

