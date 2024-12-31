#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"
//#include "Characters/CGhostTrail.h"


void FDoActionData::DoAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		if (bCanMove == false)
			movement->Stop();
	}

	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);

	//if (!!GhostTrailClass)
	//{
	//	FVector location = InOwner->GetActorLocation();
	//	location.Z -= InOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	//	FActorSpawnParameters params;
	//	params.Owner = InOwner;
	//	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	//	FTransform transform;
	//	transform.SetTranslation(location);

	//	//GhostTrail = InOwner->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, params);
	//}
}

void FDoActionData::Destroy_GhostTrail()
{
	//if (!!GhostTrail)
	//	GhostTrail->Destroy();
}


///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.HitData = this;
	
	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayHitStop(UWorld* InWorld)
{
	CheckTrue(FMath::IsNearlyZero(StopTime));


	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (!!character)
		{
			character->CustomTimeDilation = 1e-3f;

			characters.Add(character);
		}
	}


	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]() {
		for (ACharacter* character : characters)
		{
			if (!!character)
				character->CustomTimeDilation = 1.0f;
		}
	});


	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	if (!!Effect)
	{
		FTransform transform;
		transform.SetLocation(EffectLocation);
		transform.SetScale3D(EffectScale);
		transform.AddToTranslation(InLocation);

		UGameplayStatics::SpawnEmitterAtLocation(InWorld, Effect, transform);
	}

	if (!!Niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, Niagara, ownerLocation);
	}
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator)
{
	if (!!Effect)
	{

		FTransform transform;
		transform.SetLocation(InLocation + InRotator.RotateVector(EffectLocation));
		transform.SetScale3D(EffectScale);
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, Effect, transform);
	}

	if (!!Niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, Niagara, ownerLocation, InRotator);
	}
}

///////////////////////////////////////////////////////////////////////////////