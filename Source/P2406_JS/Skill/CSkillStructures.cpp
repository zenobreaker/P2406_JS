#include "Skill/CSkillStructures.h"
#include "Global.h"
#include "Weapons/CWeaponStructures.h" 
#include "Gameframework/Character.h"

void FSkillActionData::DoAction(ACharacter* InOwner)
{
	Super::DoAction(InOwner);

	// 애니메이션이 있으면 실행

	// 이펙트가 있다면 그것도 실행
}

void FSkillActionData::Begin_Casting(ACharacter* InOwner)
{
	CheckNull(InOwner);


	if (!!BeginCastingAnimMontage)
		InOwner->PlayAnimMontage(BeginCastingAnimMontage);
	else
	{
		
	}
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
