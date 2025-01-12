#include "Components/CGuardComponent.h"
#include "Global.h"

#include "Characters/IGuardable.h"

#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "Components/CAttackTraceComponent.h"


UCGuardComponent::UCGuardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCGuardComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	State = FHelpers::GetComponent <UCStateComponent>(OwnerCharacter);
	CheckNull(State);

	REGISTER_EVENT_WITH_REPLACE(State, OnStateTypeChanged, this, UCGuardComponent::OnStateTypeChanged);

	ATrace = FHelpers::GetComponent<UCAttackTraceComponent>(OwnerCharacter);

	CheckNull(ATrace);

	REGISTER_EVENT_WITH_REPLACE(ATrace, OnHandledTrace, this, UCGuardComponent::OnHandledTrace);

	REGISTER_EVENT_WITH_REPLACE(ATrace, OnEndTrace, this,
		UCGuardComponent::GuardComp_OnEndTrace);
}


void UCGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bCountering)
		CounterTime -= DeltaTime;
	else 
		CounterRecoveryTime -= DeltaTime; 


	CalcGuardHP(DeltaTime);

	CalcGuardCooldown(DeltaTime);
}

void UCGuardComponent::OnJustGuard()
{
	bJustTime = !bJustTime;
}

void UCGuardComponent::StartGuard()
{
	CheckFalse(bCanGuard);
	CheckTrue(bGuarding);

	IIGuardable* guardable = Cast<IIGuardable>(OwnerCharacter);
	CheckNull(guardable);

	bGuarding = true;
	bJustTime = false;

	GuardHP = MaxGuardHP;

	DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedGuardVisiable, true);

	guardable->StartGuard();
	OwnerCharacter->PlayAnimMontage(GuardMontage);

	CheckNull(State);
	State->SetGuardMode();
}

void UCGuardComponent::StopGuard()
{
	bGuarding = false;

	DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedGuardVisiable, false);

	IIGuardable* guardable = Cast<IIGuardable>(OwnerCharacter);
	CheckNull(guardable);

	guardable->StopGuard();
	OwnerCharacter->StopAnimMontage(GuardMontage);

	CheckNull(State);
	State->SetIdleMode();
}

bool UCGuardComponent::CheckBlocking(ACBaseCharacter::FDamageData& InDamageData)
{
	//CheckNullResult(OwnerCharacter, false);
	if (OwnerCharacter == nullptr)
		return false; 

	if (GuardHP <= 0.0f)
	{
		CalcGuardHP();

		return false;
	}

	FVector attackerLocation = InDamageData.Attacker->GetActorLocation();

	FVector toAttack = (attackerLocation - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	FVector forward = OwnerCharacter->GetActorForwardVector();

	DebugLine(toAttack, forward);

	float dotProduct = FVector::DotProduct(forward, toAttack);
	if (dotProduct > FMath::Cos(FMath::DegreesToRadians(GuardAngle)))
	{
		UGameplayStatics::PlaySoundAtLocation
		(
			GetWorld(),
			GuardSound,
			OwnerCharacter->GetActorLocation()
		);

		GuardHP += (MaxGuardHP * 0.1f) * -1.0f;

		Evaluate_JustTime();


		DYNAMIC_EVENT_CALL(OnGuardDamaged);

		return true;
	}

	return false;
}

void UCGuardComponent::CalcGuardHP(const float InDeltaTime)
{
	if (bGuarding == false)
		return; 
	
	GuardHP -= InDeltaTime;

	if (GuardHP <= 0.0f)
	{
		// 체력이 다 닳아서 깨진 가드면 바로 가드 못하게 
		bCanGuard = false;
		GuardCooldown = MaxGuardCooldown;

		StopGuard();
	}

	DYNAMIC_EVENT_CALL_TWO_PARAM(OnUpdatedGuardGauge, GuardHP, MaxGuardHP);
}

void UCGuardComponent::CalcGuardCooldown(const float InDeltaTime)
{
	if (bCanGuard)
		return; 

	if (GuardCooldown > 0.0f)
		GuardCooldown -= InDeltaTime;

	if (GuardCooldown <= 0.0f)
		bCanGuard = true;
}

//-----------------------------------------------------------------------------

void UCGuardComponent::StartCounterGuard()
{
	CheckTrue(bCountering);
	CheckTrue(CounterRecoveryTime > 0.0f);
	CheckTrue(CounterTime > 0.0f);
	
	CounterTime = MaxCounterTime;

	bCountering = true; 
	
	OwnerCharacter->PlayAnimMontage(CounterWaitMontage);
}

void UCGuardComponent::StopCounterGuard()
{

	CounterRecoveryTime = MaxCounterRecoveryTime;

	bCountering = false;

	OwnerCharacter->StopAnimMontage(CounterWaitMontage);
}

//-----------------------------------------------------------------------------

void UCGuardComponent::OnHandledTrace(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	// 반격 시 충돌 처리	
	 // 캐릭터 여부 확인

	CheckNull(InAttackCauser);

	HitData.SendDamage(InAttacker, InAttackCauser, InOther);
	//// 필요한 추가 처리
	/*Weapon->GetAttachment()->HandleAttachmentOverlap(OwnerCharacter, Weapon->GetAttachment(), InOther);*/
}

void UCGuardComponent::GuardComp_OnEndTrace()
{
	Hits.Empty();
}

void UCGuardComponent::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	if (InPrevType == EStateType::Guard && InPrevType != InNewType)
	{
		StopGuard();
	}
}

void UCGuardComponent::Evaluate_JustTime()
{
	if (bJustTime == false)
		return;

	//OwnerCharacter->PlayAnimMontage(ParryMontage);
	ParryActionData.DoAction(OwnerCharacter);
}


//-----------------------------------------------------------------------------
//Debug 

void UCGuardComponent::DebugLine(FVector InAttack, FVector InForward)
{
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

	FVector leftDirection = n_forward.RotateAngleAxis(-GuardAngle, OwnerCharacter->GetActorUpVector()); // 왼쪽 방향
	FVector rightDirection = n_forward.RotateAngleAxis(GuardAngle, OwnerCharacter->GetActorUpVector()); // 오른쪽 방향

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