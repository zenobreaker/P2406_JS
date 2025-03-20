#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollisionComponent.h"
#include "CSkillCollision_Scaling.generated.h"


UCLASS()
class P2406_JS_API UCSkillCollision_Scaling : public UCSkillCollisionComponent
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
	FVector InitScale = FVector::OneVector; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
	FVector DestScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
	float Durataion;

public:
	UCSkillCollision_Scaling();

public:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 충돌 시작
	void ActivateCollision(int32 InIndex = 0) override;
	// 충돌 종료
	void DeactivateCollision(int32 InIndex = 0) override;

	void CheckCollision() override;

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	void DrawDebugCollisionLine() override;



private:
	class UCapsuleComponent* Capsule; 
	class UBoxComponent* Box; 

private:
	FTimerHandle TimerHandle;
	FVector CurrentScale;
	float ElapsedTime; 
};
