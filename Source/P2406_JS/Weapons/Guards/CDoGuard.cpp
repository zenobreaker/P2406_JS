#include "Weapons/Guards/CDoGuard.h"
#include "Global.h"
#include "GameFramework/Character.h"



UCDoGuard::UCDoGuard()
{

}

void UCDoGuard::BeginPlay(
	ACharacter* InOwnerCharacter,
	const FGuardData& InGuardData,
	const FGuardHitData& InGuardHitData,
	const FCounterData& InCounterData)
{
	OwnerCharacter = InOwnerCharacter;
	GuardData = InGuardData;
	GuardHitData = InGuardHitData;
	CounterData = InCounterData;
}

void UCDoGuard::Tick(float DeltaTime)
{
	if (bCountering)
		CounterTime -= DeltaTime;
	else
		CounterRecoveryTime -= DeltaTime;

	CalcGuardHP(DeltaTime);

	CalcGuardCooldown(DeltaTime);
}

void UCDoGuard::Begin_Guard()
{
	CheckNull(OwnerCharacter);

	bJustTime = false; 
	bGuarding = true; 
	GuardHP = GuardData.MaxGuardHP;

	GuardData.Begin_Guard(OwnerCharacter);
}

void UCDoGuard::End_Guard()
{
	CheckNull(OwnerCharacter);

	if (!OwnerCharacter)  // 안전 체크 추가
	{
		UE_LOG(LogTemp, Error, TEXT("End_Guard: OwnerCharacter is NULL!"));
		return;
	}

	if (this == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoGuard is nullptr!  3"));
	}


	CheckFalse(bGuarding);

	bGuarding = false;

	GuardData.End_Guard(OwnerCharacter);
}

bool UCDoGuard::CheckBlocking(FVector InForward, FVector InAttacker)
{
	CheckNullResult(OwnerCharacter, false);

	if (GuardHP <= 0.0f)
	{
		CalcGuardHP();

		return false;
	}

	float dotProduct = FVector::DotProduct(InForward, InAttacker);
	if (dotProduct > FMath::Cos(FMath::DegreesToRadians(GuardData.GuardAngle)))
	{
		GuardData.PlaySoundWave(OwnerCharacter);

		return true;
	}

	return false;
}

void UCDoGuard::Damage_Guard()
{
	GuardHP += (GuardData.MaxGuardHP * 0.1f) * -1.0f;
}

void UCDoGuard::CalcGuardHP(const float InDeltaTime)
{
	if (bGuarding == false)
		return;

	GuardHP -= InDeltaTime;
	//FLog::Print("Guard HP : " + FString::SanitizeFloat(GuardHP), 1223);
	
	if (GuardHP <= 0.0f)
	{
		// 체력이 다 닳아서 깨진 가드면 바로 가드 못하게 
		bCanGuard = false;
		GuardCooldown = GuardData.MaxGuardCooldown;

		//End_Guard();
	}
}

void UCDoGuard::CalcGuardCooldown(const float InDeltaTime)
{
	if (bCanGuard)
		return;

	if (GuardCooldown > 0.0f)
		GuardCooldown -= InDeltaTime;

	if (GuardCooldown <= 0.0f)
		bCanGuard = true;
}

void UCDoGuard::OnJustGuard()
{
	bJustTime = !bJustTime;
}

void UCDoGuard::StartCounterGuard()
{
	CheckTrue(bCountering);
	CheckTrue(CounterRecoveryTime > 0.0f);
	CheckTrue(CounterTime > 0.0f);

	CounterTime = CounterData.MaxCounterTime;

	bCountering = true;

	CounterData.DoAction_CounterWait(OwnerCharacter);
}

void UCDoGuard::StopCounterGuard()
{
	CounterRecoveryTime = CounterData.MaxCounterRecoveryTime;

	bCountering = false;
	CounterData.End_CounterWait(OwnerCharacter);
}


