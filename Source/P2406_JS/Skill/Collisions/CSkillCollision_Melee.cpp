#include "CSkillCollision_Melee.h"
#include "Global.h"

#include "Components/SphereComponent.h"

ACSkillCollision_Melee::ACSkillCollision_Melee()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("Collision");
	CheckNull(CollisionComponent);

	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACSkillCollision_Melee::OnComponentBeginOverlap);
}

void ACSkillCollision_Melee::ActivateCollision()
{

}

void ACSkillCollision_Melee::DeactivateCollision()
{

}
