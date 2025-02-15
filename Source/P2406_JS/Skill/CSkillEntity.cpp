#include "Skill/CSkillEntity.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/IDamagable.h"
#include "Characters/CBaseCharacter.h"

#include "Skill/CSkillCollisionComponent.h"
#include "Skill/Collisions/CSkillCollision_Area.h"

void ACSkillEntity::SetSkillEntityData(FSkillCollisionData InData)
{
	FLog::Log("Entity Creator");
	CreateCollisionByType(InData);
}

ACSkillEntity::ACSkillEntity()
{

}

void ACSkillEntity::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	FLog::Log("Create Success Skill Entity");
}

void ACSkillEntity::BeginPlay()
{
	Super::BeginPlay();


	ActivateCollision();
}

void ACSkillEntity::CreateCollisionByType(FSkillCollisionData InData)
{
	// 범위형 스킬콜리전 
	switch (MyType)
	{
		case ESkillCollisionType::Area:
		{
			SkillCollision = NewObject<UCSkillCollision_Area>(this, "Area");
		}
		break;
	}


	if (SkillCollision != nullptr)
	{
		SkillCollision->RegisterComponent();
		//SkillCollision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SkillCollision->SetCollisionData(OwnerCharacter, InData);
	}
}

void ACSkillEntity::ActivateCollision()
{
	CheckNull(SkillCollision);

	SkillCollision->ActivateCollision();
}

void ACSkillEntity::DeactivateCollision()
{
	CheckNull(SkillCollision);

	SkillCollision->DeactivateCollision();
}

//-----------------------------------------------------------------------------
AActor* ACSkillEntity::GetDamageSource() const
{
	return nullptr;
}
