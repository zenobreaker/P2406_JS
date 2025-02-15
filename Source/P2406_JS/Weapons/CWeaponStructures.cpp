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

void FDoActionData::End_DoAction(ACharacter* InOwner)
{
	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(InOwner);
	CheckNull(movement);

	movement->Move();
}

void FDoActionData::Destroy_GhostTrail()
{
	//if (!!GhostTrail)
	//	GhostTrail->Destroy();
}


///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, bool bFirstHit)
{
	FActionDamageEvent e;
	e.bFirstHit = bFirstHit;
	e.HitData = this;

	InOther->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayHitStop(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckTrue(FMath::IsNearlyZero(StopTime));


	TArray<AActor*> actors;
	TArray<TObjectPtr<AActor>>& arr = InCharacter->GetWorld()->GetCurrentLevel()->Actors;
	for (AActor* actor : arr)
	{
		if (actor == nullptr)
			continue;

		UMeshComponent* mesh = actor->GetComponentByClass<UMeshComponent>();
		if (mesh == nullptr)
			continue;

		if (mesh->Mobility != EComponentMobility::Movable)
			continue;

		actor->CustomTimeDilation = 1e-3f;
		actors.Add(actor);
	}


	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]() 
	{
		for (AActor* actor: actors)
		{
			if (actor != nullptr)
				actor->CustomTimeDilation = 1.0f;
		}
	});


	FTimerHandle timerHandle;
	InCharacter->GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayCameraShake(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(CameraShake);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(InCharacter->GetWorld(), 0);
	CheckNull(cameraManager);

	cameraManager->StartCameraShake(CameraShake);
}

void FHitData::PlayEffect(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(Effect);

	FVector location = InCharacter->GetActorLocation();
	FRotator rotator = InCharacter->GetActorRotation();

	location += rotator.RotateVector(EffectLocation);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetScale3D(EffectScale);

	FHelpers::PlayEffect(InCharacter->GetWorld(), Effect, transform);
}

///////////////////////////////////////////////////////////////////////////////