#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollision.h"
#include "CSkillCollision_Melee.generated.h"


UCLASS()
class P2406_JS_API ACSkillCollision_Melee 
	: public ACSkillCollision
{
	GENERATED_BODY()
	
public:
	ACSkillCollision_Melee();

public:
	// �浹 ����
	virtual void ActivateCollision() override;

	// �浹 ����
	virtual void DeactivateCollision() override;
};
