#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollisionComponent.h"
#include "CSkillCollision_Projectile.generated.h"


UCLASS()
class P2406_JS_API UCSkillCollision_Projectile : public UCSkillCollisionComponent
{
	GENERATED_BODY()

public:
	// �浹 ����
	virtual void ActivateCollision() {};

	// �浹 ����
	virtual void DeactivateCollision() { };
};
