#include "Weapons/SubActions/CSubAction_Hammer.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

UCSubAction_Hammer::UCSubAction_Hammer()
{
}

void UCSubAction_Hammer::Pressed()
{
	CheckFalse(ActionDatas.Num() > 0);

	CLog::Print("Hammer SubAction Start");

	IsCharging = true;
	ActionDatas[0].DoAction(Owner);
	
	State->SetActionMode();
}

void UCSubAction_Hammer::Released()
{
	IsCharging = false;
	StartCharging();

	//TODO: �ӽ�
	State->SetIdleMode();
}

void UCSubAction_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(IsCharging == true);

	CurrentChargeTime += DeltaTime; // ���� �ð� ����
	CurrentChargeTime = FMath::Min(CurrentChargeTime, MaxChargeTime);

	
	// ���� �ð��� 10%�̻��̸� Ư�� �ִ� ��� �� ���� 
	if (CurrentChargeTime / MaxChargeTime >= 0.1f)
	{
		// TODO: �ִ� ��Ÿ�ֿ� ���� ����� �̿��غ���?
		Owner->PlayAnimMontage(ChargeAnimMontage, 0.0f);
	}

	if (CurrentChargeTime >= MaxChargeTime)
	{
		// ���� �Ϸ� �ִ� ���
		CLog::Print("Sub Action Charge Complete", 1);
	}
}

void UCSubAction_Hammer::OnPressSpecialAction()
{

}

// ������ �ʱ�ȭ
void UCSubAction_Hammer::StartCharging()
{
	IsCharging = false;
	CurrentChargeTime = 0.0f;
}
