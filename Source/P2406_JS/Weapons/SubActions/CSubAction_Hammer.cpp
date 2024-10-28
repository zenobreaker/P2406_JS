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

	//TODO: 임시
	State->SetIdleMode();
}

void UCSubAction_Hammer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckFalse(IsCharging == true);

	CurrentChargeTime += DeltaTime; // 충전 시간 증가
	CurrentChargeTime = FMath::Min(CurrentChargeTime, MaxChargeTime);

	
	// 충전 시간이 10%이상이면 특정 애님 재생 후 멈춤 
	if (CurrentChargeTime / MaxChargeTime >= 0.1f)
	{
		// TODO: 애님 몽타주에 섹션 기능을 이용해볼까?
		Owner->PlayAnimMontage(ChargeAnimMontage, 0.0f);
	}

	if (CurrentChargeTime >= MaxChargeTime)
	{
		// 충전 완료 애님 재생
		CLog::Print("Sub Action Charge Complete", 1);
	}
}

void UCSubAction_Hammer::OnPressSpecialAction()
{

}

// 충전량 초기화
void UCSubAction_Hammer::StartCharging()
{
	IsCharging = false;
	CurrentChargeTime = 0.0f;
}
