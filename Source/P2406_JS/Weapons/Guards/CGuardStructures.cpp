#include "Weapons/Guards/CGuardStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Animation/AnimMontage.h"


void FGuardData::DoGuard(ACharacter* InOwnerCharacter) const
{
	CheckNull(InOwnerCharacter);

	InOwnerCharacter->PlayAnimMontage(GuardMontage, PlayRate);
}

void FGuardData::StopGuard(ACharacter* InOwnerCharacter) const
{
	CheckNull(InOwnerCharacter);

	InOwnerCharacter->StopAnimMontage(GuardMontage);
}

void FGuardData::PlaySoundWave(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);
	CheckNull(Sound);

	//FLog::Log("Gaurd Sound Play!");

	UWorld* world = InOwnerCharacter->GetWorld();
	FVector location = InOwnerCharacter->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FGuardData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	UParticleSystem* particle = Cast< UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect); 

	if (!!particle)
	{
		FTransform transform;
		transform.SetLocation(EffectLocation);
		transform.SetScale3D(EffectScale);
		transform.AddToTranslation(InLocation);

		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
	}

	if (!!niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, ownerLocation);
	}
}

void FGuardData::PlayEffect(
	UWorld* InWorld, 
	const FVector& InLocation, 
	const FRotator& InRotator)
{
	UParticleSystem* particle = Cast< UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	if (!!particle)
	{
		FTransform transform;
		transform.SetLocation(EffectLocation + InRotator.RotateVector(EffectLocation));
		transform.SetScale3D(EffectScale);
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
	}

	if (!!niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, ownerLocation, InRotator);
	}
}

//-----------------------------------------------------------------------------

void FGuardHitData::DoGuardHit(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);

	InOwnerCharacter->PlayAnimMontage(GuardHitMontage, PlayRate);
}

void FGuardHitData::PlaySoundWave(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);
	CheckNull(Sound);

	//FLog::Log("Gaurd Sound Play!");

	UWorld* world = InOwnerCharacter->GetWorld();
	FVector location = InOwnerCharacter->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FGuardHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	UParticleSystem* particle = Cast< UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	if (!!particle)
	{
		FTransform transform;
		transform.SetLocation(EffectLocation);
		transform.SetScale3D(EffectScale);
		transform.AddToTranslation(InLocation);

		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
	}

	if (!!niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, ownerLocation);
	}
}

void FGuardHitData::PlayEffect(UWorld* InWorld, 
	const FVector& InLocation,
	const FRotator& InRotator)
{
	UParticleSystem* particle = Cast< UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	if (!!particle)
	{
		FTransform transform;
		transform.SetLocation(EffectLocation + InRotator.RotateVector(EffectLocation));
		transform.SetScale3D(EffectScale);
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
	}

	if (!!niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, ownerLocation, InRotator);
	}
}

//-----------------------------------------------------------------------------

void FCounterData::DoAction_CounterWait(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);

	InOwnerCharacter->PlayAnimMontage(CounterWaitMontage);
}

void FCounterData::End_CounterWait(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);

	InOwnerCharacter->StopAnimMontage(CounterWaitMontage);
}

void FCounterData::PlaySoundWave(ACharacter* InOwnerCharacter)
{

	CheckNull(InOwnerCharacter);

	CheckNull(Sound);

	//FLog::Log("Gaurd Sound Play!");

	UWorld* world = InOwnerCharacter->GetWorld();
	FVector location = InOwnerCharacter->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FCounterData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	UParticleSystem* particle = Cast< UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	if (!!particle)
	{
		FTransform transform;
		transform.SetLocation(EffectLocation);
		transform.SetScale3D(EffectScale);
		transform.AddToTranslation(InLocation);

		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
	}

	if (!!niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, ownerLocation);
	}
}

void FCounterData::PlayEffect(UWorld* InWorld, 
	const FVector& InLocation, 
	const FRotator& InRotator)
{
	UParticleSystem* particle = Cast< UParticleSystem>(Effect);
	UNiagaraSystem* niagara = Cast<UNiagaraSystem>(Effect);

	if (!!particle)
	{
		FTransform transform;
		transform.SetLocation(EffectLocation + InRotator.RotateVector(EffectLocation));
		transform.SetScale3D(EffectScale);
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, particle, transform);
	}

	if (!!niagara)
	{
		FVector ownerLocation = InLocation;

		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, niagara, ownerLocation, InRotator);
	}
}

//-----------------------------------------------------------------------------

void FParryData::DoAction_Parry(ACharacter* InOwnerCharacter)
{
	CheckNull(InOwnerCharacter);

	ParryActionData.DoAction(InOwnerCharacter);
}


