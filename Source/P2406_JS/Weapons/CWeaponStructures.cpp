#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Characters/IDamagable.h"

//#include "Characters/CGhostTrail.h"


void FDoActionData::DoAction(ACharacter* InOwner, /*bool IsImmedate,*/ FName InSectionName)
{
	UCMovementComponent* movement = FHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		if (bCanMove == false)
			movement->Stop();
	}

	
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate, InSectionName);


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

void FDoActionData::AnimationPlayback(ACharacter* InOwner, float InValue)
{
	CheckNull(InOwner); 

	InOwner->GetMesh()->GetAnimInstance()->Montage_SetPlayRate(Montage, InValue);
}


///////////////////////////////////////////////////////////////////////////////

void FHitData::SendDamage(ACharacter* InAttacker, AActor* InAttackCauser, AActor* InOther, bool bFirstHit)
{
	FActionDamageEvent e;
	e.bFirstHit = bFirstHit;
	e.HitData = this;

	IIDamagable* damagable = Cast<IIDamagable>(InOther);
	if(damagable != nullptr)
		damagable->TakeDamage(Power, e, InAttacker->GetController(), InAttackCauser);
}

void FHitData::PlayHitStop(AActor* InActor)
{
	CheckNull(InActor);
	CheckTrue(FMath::IsNearlyZero(StopTime));


	TArray<AActor*> actors;
	TArray<TObjectPtr<AActor>>& arr = InActor->GetWorld()->GetCurrentLevel()->Actors;
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
	InActor->GetWorld()->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime, false);
}

void FHitData::PlaySoundWave(AActor* InActor)
{
	CheckNull(Sound);

	UWorld* world = InActor->GetWorld();
	FVector location = InActor->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FHitData::PlayCameraShake(AActor* InActor)
{
	CheckNull(InActor);
	CheckNull(CameraShake);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(InActor->GetWorld(), 0);
	CheckNull(cameraManager);

	cameraManager->StartCameraShake(CameraShake);
}

void FHitData::PlayEffect(AActor* InActor)
{
	CheckNull(InActor);
	CheckNull(Effect);

	FVector location = InActor->GetActorLocation();
	FRotator rotator = InActor->GetActorRotation();

	location += rotator.RotateVector(EffectLocation);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetScale3D(EffectScale);

	FHelpers::PlayEffect(InActor->GetWorld(), Effect, transform);
}

///////////////////////////////////////////////////////////////////////////////