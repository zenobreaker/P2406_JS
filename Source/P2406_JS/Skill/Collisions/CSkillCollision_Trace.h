#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollisionComponent.h"
#include "CSkillCollision_Trace.generated.h"



UCLASS(Blueprintable)
class P2406_JS_API UCSkillCollision_Trace : public UCSkillCollisionComponent
{
	GENERATED_BODY()

public:
	UCSkillCollision_Trace();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
	float CollisionRadius = 200.0f;


public:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 충돌 시작
	void ActivateCollision() override;
	// 충돌 종료
	void DeactivateCollision() override;

	void CheckCollision() override;

protected:
	void DrawDebugCollisionLine() override;

};
