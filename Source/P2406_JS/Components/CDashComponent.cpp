#include "Components/CDashComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CTargetComponent.h"

UCDashComponent::UCDashComponent()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!!OwnerCharacter)
	{
		movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
		target = CHelpers::GetComponent<UCTargetComponent>(OwnerCharacter); 
	}
}


void UCDashComponent::BeginPlay()
{
	Super::BeginPlay();
}



void UCDashComponent::OnDash()
{
	CheckNull(movement);

	DashAction();
}

void UCDashComponent::DashAction()
{
	CheckNull(movement);
	//CheckNull(target);

	FVector* input = movement->GetInputDirection();
	if (input == nullptr )
		return; 

	// 전방
	if(input->X > 0)
	{
		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Forward]);
	}
	// 후방 
	else if(input->X < 0)
	{
		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Back]);
	}
}

