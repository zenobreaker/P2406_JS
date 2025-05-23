#include "Components/CStateComponent.h"
#include "Global.h"

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

void UCStateComponent::SetAirborneMode()
{
	ChangeType(EStateType::Airborne);
}

void UCStateComponent::SetDownMode()
{
	ChangeType(EStateType::Down);
}

void UCStateComponent::SetGuardMode()
{
	ChangeType(EStateType::Guard);
}

void UCStateComponent::SetExecuteMode()
{
	ChangeType(EStateType::Execute);
}

FString UCStateComponent::EnumToString(EStateType State)
{
	static const TMap<EStateType, FString> EnumToStringMap =
	{
		{EStateType::Idle, TEXT("Idle")},
		{EStateType::Evade, TEXT("Evade")},
		{EStateType::Dash, TEXT("Dash")},
		{EStateType::Equip, TEXT("Equip")},
		{EStateType::Damaged, TEXT("Damaged")},
		{EStateType::Action, TEXT("Action")},
		{EStateType::Airborne, TEXT("Airborne")},
		{EStateType::Dead, TEXT("Dead")},
		{EStateType::Guard, TEXT("Guard")},
		{EStateType::Execute, TEXT("Execute")},
		{EStateType::Max, TEXT("Max")}
	};

	if (const FString* Result = EnumToStringMap.Find(State))
	{
		return *Result; 
	}

	return TEXT("Unknown");
}

void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;
	
	//FLog::Log(GetOwner()->GetName() + " " + EnumToString(Type));

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast(prevType, InType);
}

void UCStateComponent::HandleGuardState()
{

}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true; 
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}
