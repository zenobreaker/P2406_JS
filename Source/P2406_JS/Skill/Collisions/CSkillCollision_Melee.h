#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollisionComponent.h"
#include "CSkillCollision_Melee.generated.h"


UCLASS()
class P2406_JS_API UCSkillCollision_Melee 
	: public UCSkillCollisionComponent
{
	GENERATED_BODY()
	
public:
	UCSkillCollision_Melee();

public:
	virtual void BeginPlay() override; 
	virtual void SetCollision(UPrimitiveComponent* InComponent); 

	
public:
	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

//public:
//	// 충돌 시작
//	virtual void ActivateCollision() override;
//
//	// 충돌 종료
//	virtual void DeactivateCollision() override;
};
