#include "Components/CGuardComponent.h"
#include "Global.h"

UCGuardComponent::UCGuardComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCGuardComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCGuardComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCGuardComponent::StartGuard()
{
	CheckFalse(bCanGuard);

	bGuarding = true; 

	GuardHP = MaxGuardHP;
}

void UCGuardComponent::StopGuard()
{
	bGuarding = false; 
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


		DYNAMIC_EVENT_CALL(OnGuardDamaged);
		
		DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedGuardVisiable, true);
		if (OnUpdatedGuardVisiable.IsBound())
			OnUpdatedGuardVisiable.Broadcast(true);

		DYNAMIC_EVENT_CALL_TWO_PARAM(OnUpdatedGuardGauge, GuardHP, MaxGuardHP);
		/*if (OnUpdatedGuardGauge.IsBound())
			OnUpdatedGuardGauge.Broadcast(GuardHP, MaxGuardHP); */

		return true;
	}

	return false;
}

