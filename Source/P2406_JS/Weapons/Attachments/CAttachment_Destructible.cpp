#include "Weapons/Attachments/CAttachment_Destructible.h"
#include "Global.h"

#include "Components/CHealthPointComponent.h"
#include "Damages/CDamageHandler.h"

ACAttachment_Destructible::ACAttachment_Destructible()
{
	FHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "HealthPoint");
	FHelpers::CreateActorComponent<UCDamageHandler>(this, &DamageHandler, "Damage");
}

void ACAttachment_Destructible::BeginPlay()
{
	//Super::BeginPlay();

	FLog::Log("Begin Object Destruct");

	if (HealthPoint)
		HealthPoint->BeginPlay(); 
	if (DamageHandler)
		DamageHandler->BeginPlay();
}

void ACAttachment_Destructible::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	FLog::Log("End Object Destruct");
	
}


float ACAttachment_Destructible::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	CheckNullResult(DamageHandler, Damage);

	DamageHandler->ApplyDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (!!HealthPoint && HealthPoint->IsDead())
	{
		FLog::Log("Distructible Object Dead");
		this->Destroy(); 
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		DYNAMIC_EVENT_CALL(OnDestroyedObject);
	}

	return Damage;
}