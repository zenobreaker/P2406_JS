#include "Components/CConditionComponent.h"
#include "Characters/IConditionBase.h"
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

void UCConditionComponent::AddCondition(EConditionState NewCondition)
{
    // ���ο��� ���� ����� �ٸ��� �˻� 
    bool check = true;
    check = HasCondition(NewCondition);
    if (check)
    {
        // ��������Ʈ ȣ�� (���� ���� �� ĳ���Ϳ��� �˸���)
        //OnConditionChanged.Broadcast(NewCondition);
        if (NewCondition == EConditionState::CONDITION_DOWNED)
        {
            // �ش� ����� �������̽��� ������ �װ� �� �� 
            if (IDownCondition* DownCondition = Cast<IDownCondition>(GetOwner()))
                DownCondition->OnDownConditionActivated();
        }

        // EConditionState�� ��Ÿ�� �������̹Ƿ� ���������� ��ȯ�ؾ��Ѵ�. 
        Condition = static_cast<EConditionState>(static_cast<uint8>(Condition)
            | static_cast<uint8>(NewCondition));
    }

}

void UCConditionComponent::RemoveCondition(EConditionState NewCondition)
{
    bool check = true;
    check = HasCondition(NewCondition);
    if (check)
    {
        if (NewCondition == EConditionState::CONDITION_DOWNED)
        {
            // �ش� ����� �������̽��� ������ �װ� �� �� 
            if (IDownCondition* DownCondition = Cast<IDownCondition>(GetOwner()))
                DownCondition->OnDownConditionDeactivated();
        }

        Condition = static_cast<EConditionState>(static_cast<uint8>(Condition) & ~static_cast<uint8>(NewCondition));
    }
}
  

