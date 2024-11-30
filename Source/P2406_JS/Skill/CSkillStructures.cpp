#include "Skill/CSkillStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponStructures.h" 
#include "Skill/CSkillCollision.h"
#include "Gameframework/Character.h"

void FSkillActionData::DoAction(ACharacter* InOwner)
{
	Super::DoAction(InOwner);

	// 애니메이션이 있으면 실행
	InOwner->PlayAnimMontage(Montage);
	// 이펙트가 있다면 그것도 실행
}

void FSkillActionData::Create_SkillCollision(ACharacter* InOwner)
{
	CLog::Print("Create Skill Collision");

	CheckNull(SkillCollisionClass);

	FActorSpawnParameters param;
	param.Owner = InOwner;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ACSkillCollision* skillCollision = InOwner->GetWorld()->SpawnActor<ACSkillCollision>(SkillCollisionClass, InOwner->GetOwner()->GetActorLocation(), 
		InOwner->GetOwner()->GetActorRotation(),
		param);
}

void FSkillActionData::Begin_Casting(ACharacter* InOwner)
{
	CheckNull(InOwner);

	InOwner->PlayAnimMontage(BeginCastingAnimMontage);
}

void FSkillActionData::DoCasting(ACharacter* InOwner)
{
	CheckNull(InOwner);

	InOwner->PlayAnimMontage(CastingAnimMontage);
}

void FSkillActionData::End_Casting(ACharacter* InOwner)
{
	CheckNull(InOwner);

	InOwner->PlayAnimMontage(EndCastingAnimMontage);
}

void FSkillActionData::Destroy_GhostTrail()
{
	Super::Destroy_GhostTrail();
}
