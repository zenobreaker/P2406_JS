#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollision.h"
#include "CSkillCollision_Area.generated.h"


UCLASS()
class P2406_JS_API ACSkillCollision_Area : public ACSkillCollision
{
	GENERATED_BODY()

public:
	ACSkillCollision_Area();
private:
	UPROPERTY(VisibleAnywhere)
	class USphereComponent* AreaCollisionComponent;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Size")
	float CollisionRadius = 100.0f;

public:
	// �浹 ����
	virtual void ActivateCollision() override;

	// �浹 ����
	virtual void DeactivateCollision() override;
};
