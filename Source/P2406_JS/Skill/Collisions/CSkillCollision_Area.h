#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollisionComponent.h"
#include "CSkillCollision_Area.generated.h"


UCLASS()
class P2406_JS_API UCSkillCollision_Area : public UCSkillCollisionComponent
{
	GENERATED_BODY()

public:
	UCSkillCollision_Area();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Size")
	float CollisionRadius = 200.0f;


public:
	void BeginPlay() override; 

public:
	// 충돌 시작
	virtual void ActivateCollision() override;
	// 충돌 종료
	virtual void DeactivateCollision() override;

	virtual void CheckCollision() override; 

private:
	FVector StartLocation;
	FTimerHandle TimerHandle;
};
