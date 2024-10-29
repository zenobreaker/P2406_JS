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
	
	ResetCharging();

	IsCharging = true;
	//ActionDatas[0].DoAction(Owner);
	Owner->PlayAnimMontage(ChargeAnimMontage, 0.0f);

	//State->SetActionMode();
}

void UCSubAction_Hammer::Released()
{

	// ���� ���°� �Ϸ� �Ǽ� ���� �����̸� 
	if (bActionable == true)
	{
		// ���� �׼� ����
		ActionDatas[0].DoAction(Owner);

		State->SetIdleMode();
		bActionable = false; 
		return;  
	}
	else 
		Owner->PlayAnimMontage(ChargeAnimMontage, 1.0f);

	ResetCharging();
}

void UCSubAction_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(IsCharging == true);

	CurrentChargeTime += DeltaTime; // ���� �ð� ����
	CurrentChargeTime = FMath::Min(CurrentChargeTime, MaxChargeTime);

	if (CurrentChargeTime >= MaxChargeTime)
	{
		// ���� �Ϸ� �ִ� ���
		CLog::Print("Sub Action Charge Complete", 1, 2);
		bActionable = true;
	}
}


// ������ �ʱ�ȭ
void UCSubAction_Hammer::ResetCharging()
{
	IsCharging = false;
	bActionable = false;
	CurrentChargeTime = 0.0f;
}
