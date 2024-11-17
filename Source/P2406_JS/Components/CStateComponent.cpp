#include "Components/CStateComponent.h"

UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCStateComponent::SetIdleMode()
{
	ChangeType(EStateType::Idle);
}

void UCStateComponent::SetDashMode()
{
	ChangeType(EStateType::Dash);
}

void UCStateComponent::SetEvadeMode()
{
	ChangeType(EStateType::Evade);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::SetDamagedMode()
{
	ChangeType(EStateType::Damaged);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionborneMode()
{
	ChangeType(EStateType::Airborne);
}

void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InType);

}



void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true; 
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}
