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



//TODO: �ٿ���´� ��� ���� �ð��� ������ �Ͼ�� �Ѵ�. => �̷��� ���� �����͸� �����ؾ��Ұ�

bool UCConditionComponent::HasCondition(EConditionState InCondition)
{
	// ���� ���¿� ������ ���°� ���ԵǾ� �ִ��� Ȯ�� (��Ʈ AND)
	return (static_cast<uint8>(Condition) & static_cast<uint8>(InCondition)) != 0;
}

bool UCConditionComponent::IsSameCondition(EConditionState InCondition1, EConditionState InCondition2)
{
	return (static_cast<uint8>(InCondition1) & static_cast<uint8>(InCondition2)) != 0;;
}

void UCConditionComponent::AddCondition(EConditionState NewCondition)
{
	// ���ο��� ���� ����� �ٸ��� �˻� 
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
			// �ش� ����� �������̽��� ������ �װ� �� �� 
			if (IIDownable* DownCondition = Cast<IIDownable>(GetOwner()))
				DownCondition->OnDownConditionActivated();
		}

		// EConditionState�� ��Ÿ�� �������̹Ƿ� ���������� ��ȯ�ؾ��Ѵ�. 
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
			// �ش� ����� �������̽��� ������ �װ� �� �� 
			if (IIDownable* DownCondition = Cast<IIDownable>(GetOwner()))
				DownCondition->OnDownConditionDeactivated();
		}

		Condition = static_cast<EConditionState>(static_cast<uint8>(Condition) & ~static_cast<uint8>(NewCondition));

		if (OnRemoveCondiitionType.IsBound())
			OnRemoveCondiitionType.Broadcast(NewCondition);
	}
}


