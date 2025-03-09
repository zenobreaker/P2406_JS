#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollisionComponent.h"
#include "CSkillCollision_Shape.generated.h"



UCLASS()
class P2406_JS_API UCSkillCollision_Shape : public UCSkillCollisionComponent
{
	GENERATED_BODY()

public:
	UCSkillCollision_Shape();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Size")
	float CollisionRadius = 200.0f;


public:
	void BeginPlay() override; 
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// �浹 ����
	void ActivateCollision() override;
	// �浹 ����
	void DeactivateCollision() override;

	void CheckCollision() override; 

protected:
	void DrawDebugCollisionLine() override;

private:
	FTimerHandle TimerHandle;
};
