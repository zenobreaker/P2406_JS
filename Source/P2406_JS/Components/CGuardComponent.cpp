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
	ATrace = FHelpers::GetComponent<UCAttackTraceComponent>(OwnerCharacter);

	CheckNull(ATrace);

	REGISTER_EVENT_WITH_REPLACE(ATrace, OnHandledTrace, this, UCGuardComponent::OnHandledTrace);

	REGISTER_EVENT_WITH_REPLACE(ATrace, OnEndTrace, this,
		UCGuardComponent::GuardComp_OnEndTrace);
}


void UCGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCGuardComponent::OnJustGuard()
{
	bJustTime = !bJustTime;
}

void UCGuardComponent::StartGuard()
{
	CheckFalse(bCanGuard);

	bGuarding = true;
	bJustTime = false;

	GuardHP = MaxGuardHP;

	IIGuardable* guardable = Cast<IIGuardable>(OwnerCharacter);
	CheckNull(guardable);

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
	CheckNullResult(OwnerCharacter, false);

	if (GuardHP <= 0.0f)
		return false;


	FVector attackerLocation = InDamageData.Attacker->GetActorLocation();

	FVector toAttack = (attackerLocation - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	FVector forward = OwnerCharacter->GetActorForwardVector();

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

		DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedGuardVisiable, true);
		//if (OnUpdatedGuardVisiable.IsBound())
		//	OnUpdatedGuardVisiable.Broadcast(true);

		DYNAMIC_EVENT_CALL_TWO_PARAM(OnUpdatedGuardGauge, GuardHP, MaxGuardHP);
		/*if (OnUpdatedGuardGauge.IsBound())
			OnUpdatedGuardGauge.Broadcast(GuardHP, MaxGuardHP); */

		return true;
	}

	return false;
}

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

void UCGuardComponent::Evaluate_JustTime()
{
	//if (bJustTime == false)
	//	return; 

	//OwnerCharacter->PlayAnimMontage(ParryMontage);
	ParryActionData.DoAction(OwnerCharacter);
}

