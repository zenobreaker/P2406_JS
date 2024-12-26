#include "Components/CConditionComponent.h"
#include "Characters/Condition_Interfaces/IAirborne.h"
#include "Characters/Condition_Interfaces/IDownable.h"
#include "Global.h"
#include "Gameframework/Character.h"

UCConditionComponent::UCConditionComponent()
{

}

void UCConditionComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCConditionComponent::AddAirborneCondition()
{
	AddCondition(EConditionState::CONDITION_AIRBORNE);
}

void UCConditionComponent::RemoveAirborneCondition()
{
	RemoveCondition(EConditionState::CONDITION_AIRBORNE);
}

void UCConditionComponent::AddDownCondition()
{
	AddCondition(EConditionState::CONDITION_DOWNED);
}

void UCConditionComponent::RemoveDownCondition()
{
	RemoveCondition(EConditionState::CONDITION_DOWNED);
}



//TODO: 다운상태는 어느 정도 시간이 지나면 일어나야 한다. => 이러한 정보 데이터를 구현해야할것

bool UCConditionComponent::HasCondition(EConditionState InCondition)
{
	// 현재 상태에 지정된 상태가 포함되어 있는지 확인 (비트 AND)
	return (static_cast<uint8>(Condition) & static_cast<uint8>(InCondition)) != 0;
}

bool UCConditionComponent::IsSameCondition(EConditionState InCondition1, EConditionState InCondition2)
{
	return (static_cast<uint8>(InCondition1) & static_cast<uint8>(InCondition2)) != 0;;
}

void UCConditionComponent::AddCondition(EConditionState NewCondition)
{
	// 새로오는 값이 현재와 다른지 검사 
	bool check = true;
	check = HasCondition(NewCondition);
	if (check == false)
	{
		
		if(IsSameCondition(NewCondition, EConditionState::CONDITION_AIRBORNE) == true)
		{
			if (IIAirborne* AirborneCondition = Cast<IIAirborne>(GetOwner()))
				AirborneCondition->OnAirborneConditionActivated();
		}

		if (IsSameCondition(NewCondition, EConditionState::CONDITION_DOWNED) == true)
		{
			// 해당 대상이 인터페이스가 있으면 그걸 콜 함 
			if (IIDownable* DownCondition = Cast<IIDownable>(GetOwner()))
				DownCondition->OnDownConditionActivated();
		}

		// EConditionState가 강타입 열거형이므로 정수형으로 변환해야한다. 
		Condition = static_cast<EConditionState>(static_cast<uint8>(Condition)
			| static_cast<uint8>(NewCondition));

		if (OnAddCondiitionType.IsBound())
			OnAddCondiitionType.Broadcast(NewCondition);
	}

}

void UCConditionComponent::RemoveCondition(EConditionState NewCondition)
{
	bool check = true;
	check = HasCondition(NewCondition);
	if (check == true)
	{
		if (IsSameCondition(NewCondition, EConditionState::CONDITION_AIRBORNE) == true)
		{
			if (IIAirborne* AirborneCondition = Cast<IIAirborne>(GetOwner()))
				AirborneCondition->OnAirborneConditionDeactivated();
		}

		if (IsSameCondition(NewCondition, EConditionState::CONDITION_DOWNED) == true)
		{
			// 해당 대상이 인터페이스가 있으면 그걸 콜 함 
			if (IIDownable* DownCondition = Cast<IIDownable>(GetOwner()))
				DownCondition->OnDownConditionDeactivated();
		}

		Condition = static_cast<EConditionState>(static_cast<uint8>(Condition) & ~static_cast<uint8>(NewCondition));

		if (OnRemoveCondiitionType.IsBound())
			OnRemoveCondiitionType.Broadcast(NewCondition);
	}
}


