#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollision.h"
#include "CSkillCollision_Projectile.generated.h"


UCLASS()
class P2406_JS_API ACSkillCollision_Projectile : public ACSkillCollision
{
	GENERATED_BODY()
	
public:
	// �浹 ����
	virtual void ActivateCollision() override;

	// �浹 ����
	virtual void DeactivateCollision() override;
};
