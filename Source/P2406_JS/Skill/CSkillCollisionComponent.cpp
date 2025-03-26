#include "Skill/CSkillCollisionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Skill/CSkillEntity.h"
#include "Characters/IDamagable.h"
#include "Characters/CBaseCharacter.h"

float UCSkillCollisionComponent::GetNextDelay()
{
	return CollisionData.NextDelay;
}

void UCSkillCollisionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCSkillCollisionComponent::SetCollisionData(ACharacter* InOwner, FSkillCollisionData& InCollisinData)
{
	OwnerCharacter = InOwner; 
	CollisionData = InCollisinData;

	HitData = CollisionData.HitData;
}

void UCSkillCollisionComponent::SetCollisionData(ACharacter* InOwner, FSkillCollisionData& InCollisinData, 
	ACSkillEntity* InEntity, UFXSystemAsset* InAsset)
{
	SetCollisionData(InOwner, InCollisinData);

	Entity = InEntity;
	UFXSystemAsset* sfxAsset = InCollisinData.GetSkillEffectAsset();
	CheckNull(sfxAsset);
	
	if(sfxAsset->IsA<UParticleSystem>())
	{
		Particle = NewObject<UParticleSystemComponent>(this);
		Particle->SetTemplate(Cast<UParticleSystem>(sfxAsset));
		Particle->RegisterComponent();
	}
	else if (sfxAsset->IsA<UNiagaraSystem>())
	{
		Niagara = NewObject<UNiagaraComponent>(this);
		Niagara->SetAsset(Cast<UNiagaraSystem>(sfxAsset));
		Niagara->RegisterComponent();
	}
}

void UCSkillCollisionComponent::ActivateCollision()
{
	CollisionData.Play_SkillEffect(Entity); 
}

void UCSkillCollisionComponent::DeactivateCollision()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}


//-----------------------------------------------------------------------------

bool UCSkillCollisionComponent::CheckMyTeam(AActor* InOtherActor)
{
	ACBaseCharacter* character = Cast<ACBaseCharacter>(InOtherActor);
	CheckNullResult(character, false);
	auto OtherTeamID = character->GetGenericTeamId();

	ACBaseCharacter* myBase = Cast<ACBaseCharacter>(OwnerCharacter);
	CheckNullResult(myBase, false);

	auto myTeamid = myBase->GetGenericTeamId();

	CheckTrueResult(FGenericTeamId::GetAttitude(OtherTeamID, myTeamid) == ETeamAttitude::Hostile, false);

	return true;
}

AActor* UCSkillCollisionComponent::GetDamageSource() const
{
	return nullptr;
}
