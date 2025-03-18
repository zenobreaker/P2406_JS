#include "Components/CGuardComponent.h"
#include "Global.h"

#include "Characters/IGuardable.h"
#include "Characters/CPlayer.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAttackTraceComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CWeaponComponent.h"

#include "Weapons/Guards/CGuardDataAsset.h"
#include "Weapons/Guards/CGuardData.h"
#include "Weapons/Guards/CDoGuard.h"
#include "Weapons/Guards/CDoParry.h"

//#define LOG_UCGuardComponent

UCGuardComponent::UCGuardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCGuardComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	CheckNull(GuardAsset);
	GuardAsset->BeginPlay(OwnerCharacter, &GuardData);

	if (!!GuardData)
	{
		DoGuard = GuardData->GetGuard();
		DoParry = GuardData->GetParry();
	}

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	if (!!State)
	{
		REGISTER_EVENT_WITH_REPLACE(
			State
			, OnStateTypeChanged
			, this
			, UCGuardComponent::OnStateTypeChanged);
	}

	ATrace = FHelpers::GetComponent<UCAttackTraceComponent>(OwnerCharacter);
	if (!!ATrace)
	{
		REGISTER_EVENT_WITH_REPLACE(
			ATrace
			, OnHandledParryTrace
			, this
			, UCGuardComponent::OnHandledParryTrace);

		REGISTER_EVENT_WITH_REPLACE(
			ATrace
			, OnEndTrace
			, this
			, UCGuardComponent::GuardComp_OnEndTrace);
	}

	Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
	{
		REGISTER_EVENT_WITH_REPLACE(Weapon, OnEndedDoAction, this, UCGuardComponent::End_Parry);
	}

	Move = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
}


void UCGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DoGuard != nullptr)
		DoGuard->Tick(DeltaTime);

	CalcGuardHP(DeltaTime);
}

bool UCGuardComponent::GetCanGuard() const
{
	CheckNullResult(DoGuard, false);

	return DoGuard->GetCanGuard();
}

bool UCGuardComponent::GetGuarding() const
{
	CheckNullResult(DoGuard, false);

	return DoGuard->GetGuarding();
}

bool UCGuardComponent::GetCountering() const
{
	CheckNullResult(DoGuard, false);

	return DoGuard->GetCountering();
}

void UCGuardComponent::SetCanGuard(const bool InValue)
{
	CheckNull(DoGuard);

	DoGuard->SetCanGuard(InValue);
}

void UCGuardComponent::OnJustGuard()
{
	CheckNull(DoGuard);

	DoGuard->OnJustGuard();
}

void UCGuardComponent::StartGuard()
{
	CheckNull(OwnerCharacter);
	CheckNull(State);
	CheckFalse(State->IsIdleMode());

	CheckNull(DoGuard);
	//bCanGuard
	CheckFalse(DoGuard->GetCanGuard());

	//bGuarding
	CheckTrue(DoGuard->GetGuarding());

	IIGuardable* guardable = Cast<IIGuardable>(OwnerCharacter);
	CheckNull(guardable);
	guardable->StartGuard();


	DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedGuardVisiable, true);

	if (!!DoGuard)
		DoGuard->Begin_Guard();

	if(!!State)
		State->SetGuardMode();

	if (Move != nullptr && DoGuard->GetCanMove() == false)
		Move->Stop();

	//prevRotationYaw = OwnerCharacter->bUseControllerRotationYaw ;  
	//// 가드 중 회전 방지
	//OwnerCharacter->bUseControllerRotationYaw = false;
}

void UCGuardComponent::StopGuard()
{
	CheckNull(OwnerCharacter);

	DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedGuardVisiable, false);

	IIGuardable* guardable = Cast<IIGuardable>(OwnerCharacter);
	CheckNull(guardable);
	guardable->StopGuard();

	if (DoGuard == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoGuard is nullptr!  2 "));
	}

	if (DoGuard != nullptr)
	{
		CheckFalse(DoGuard->GetGuarding());

		DoGuard->End_Guard();
	}

	if (!!State)
		State->SetIdleMode();

	if (!!Move)
		Move->Move();

	//OwnerCharacter->bUseControllerRotationYaw = prevRotationYaw;
}

bool UCGuardComponent::CheckBlocking(class ACharacter* InAttacker)
{
	CheckNullResult(DoGuard, false);
	CheckNullResult(InAttacker, false);
	CheckFalseResult(DoGuard->GetGuarding(), false);

	// 이미 저스트 타이밍에 걸렸다면 다음 판정은 하지 않음.
	if (DoParry->GetParring() == true)
		return true; 

	FVector attackerLocation = InAttacker->GetActorLocation();

	FVector toAttack = (attackerLocation - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	FVector forward = OwnerCharacter->GetActorForwardVector();
#ifdef LOG_UCGuardComponent
	DebugLine(toAttack, forward);
#endif


	if (DoGuard->CheckBlocking(forward, toAttack) == true)
	{
		Evaluate_JustTime();

		DoGuard->Damage_Guard();

		DYNAMIC_EVENT_CALL(OnGuardDamaged);

		return true;
	}

	return false;
}

void UCGuardComponent::CalcGuardHP(const float InDeltaTime)
{
	CheckNull(OwnerCharacter);
	CheckNull(GuardData);
	CheckNull(DoGuard);
	
	CheckNull(State);
	CheckTrue(State->IsDeadMode());
	if (DoGuard == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DoGuard is nullptr!  1 "));
	}

	CheckFalse(DoGuard->GetGuarding());

	DYNAMIC_EVENT_CALL_TWO_PARAMS(
		OnUpdatedGuardGauge,
		DoGuard->GetGuardHP(),
		DoGuard->GetMaxGuardHP());

	if (DoGuard->GetGuardHP() <= 0.0f)
	{
		StopGuard();
	}

}

//-----------------------------------------------------------------------------

void UCGuardComponent::StartCounterGuard()
{
	CheckNull(DoGuard);

	DoGuard->StartCounterGuard();
}

void UCGuardComponent::StopCounterGuard()
{
	CheckNull(DoGuard);

	DoGuard->StopCounterGuard();
}

void UCGuardComponent::Evaluate_JustTime()
{
	CheckNull(DoGuard);
	if (DoGuard->GetJustTime() == false)
		return;

	Begin_Parry();
}

void UCGuardComponent::Begin_Parry()
{
	CheckNull(DoParry);

	DoParry->DoAction_Parry();

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	*player->bCountering = DoParry->GetParring();
}

void UCGuardComponent::End_Parry()
{
	CheckNull(DoParry);

	//FLog::Log("Guard End Do Parry");

	DoParry->End_DoAction_Parry();

	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	*player->bCountering = DoParry->GetParring();
}

//-----------------------------------------------------------------------------

void UCGuardComponent::OnHandledParryTrace(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	// 반격 시 충돌 처리	
	 // 캐릭터 여부 확인
	CheckNull(InAttackCauser);
	CheckNull(DoParry);

	DoParry->OnHandledTrace(InAttacker, InAttackCauser, InOther);
}

void UCGuardComponent::GuardComp_OnEndTrace()
{
	CheckNull(DoParry);

	DoParry->EndTrace();
}

void UCGuardComponent::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	if (InPrevType == EStateType::Guard && InPrevType != InNewType)
	{
		StopGuard();
	}
}




//-----------------------------------------------------------------------------
//Debug 

void UCGuardComponent::DebugLine(FVector InAttack, FVector InForward)
{
	CheckNull(DoGuard);

	float guardAngle = DoGuard->GetGuardAngle();

	DrawDebugLine(
		OwnerCharacter->GetWorld(),
		OwnerCharacter->GetActorLocation(),
		OwnerCharacter->GetActorLocation() + InForward * 300.0f,
		FColor::Blue,
		false,
		1.0f);

	DrawDebugLine(
		OwnerCharacter->GetWorld(),
		OwnerCharacter->GetActorLocation(),
		OwnerCharacter->GetActorLocation() + InAttack * 300.0f,
		FColor::Red,
		false,
		1.0f);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector n_forward = FQuat(rotator).GetForwardVector();

	FVector leftDirection = n_forward.RotateAngleAxis(-guardAngle, OwnerCharacter->GetActorUpVector()); // 왼쪽 방향
	FVector rightDirection = n_forward.RotateAngleAxis(guardAngle, OwnerCharacter->GetActorUpVector()); // 오른쪽 방향

	DrawDebugLine(
		OwnerCharacter->GetWorld(),
		OwnerCharacter->GetActorLocation(),
		OwnerCharacter->GetActorLocation() + leftDirection * 300.0f,
		FColor::Green,
		false,
		1.0f);



	DrawDebugLine(
		OwnerCharacter->GetWorld(),
		OwnerCharacter->GetActorLocation(),
		OwnerCharacter->GetActorLocation() + rightDirection * 300.0f,
		FColor::Green,
		false,
		1.0f);

}