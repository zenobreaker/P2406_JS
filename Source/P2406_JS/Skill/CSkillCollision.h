#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkillCollision.generated.h"

UCLASS(Abstract)
class P2406_JS_API ACSkillCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	ACSkillCollision();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill")
	class AActor* SkillOwner;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Category = "Skill")
	float Damage; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float Duration; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float CollisionSize;


protected:
	virtual void BeginPlay() override;

public:
	// 충돌 시작
	UFUNCTION(BlueprintCallable, Category = "Skill Collision")
	virtual void ActivateCollision();

	// 충돌 종료
	UFUNCTION(BlueprintCallable, Category = "Skill Collision")
	virtual void DeactivateCollision();

protected:
	// 충돌이 발생했을 때 처리할 이벤트
	UFUNCTION()
	virtual void HandleCollision(AActor* HitActor);

public:
	TArray<class ACharacter*> Hitted;

};
