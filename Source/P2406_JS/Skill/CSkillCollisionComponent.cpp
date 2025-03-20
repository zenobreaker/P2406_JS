#include "Skill/CSkillCollisionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/IDamagable.h"
#include "Characters/CBaseCharacter.h"

void UCSkillCollisionComponent::BeginPlay()
{
	Super::BeginPlay();

	Index = 0;
}

void UCSkillCollisionComponent::SetCollisionData(ACharacter* InOwner, FSkillCollisionData& InCollisinData)
{
	OwnerCharacter = InOwner; 
	CollisionData = InCollisinData;

	HitDatas = CollisionData.HitDatas;
}

void UCSkillCollisionComponent::SetCollisionData(ACharacter* InOwner, FSkillCollisionData& InCollisinData, ACSkillEntity* InEntity)
{
	SetCollisionData(InOwner, InCollisinData);

	Entity = InEntity;
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
