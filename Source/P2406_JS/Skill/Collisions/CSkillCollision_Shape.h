#pragma once

#include "CoreMinimal.h"
#include "Skill/CSkillCollisionComponent.h"
#include "CSkillCollision_Shape.generated.h"

UENUM(BlueprintType)
enum class EScaleType : uint8 
{
	AOE, Laser_Axis_X, Max,
};

UCLASS(Blueprintable)
class P2406_JS_API UCSkillCollision_Shape : public UCSkillCollisionComponent
{
	GENERATED_BODY()
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Type")
	EScaleType Type = EScaleType::Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
	FVector InitScale = FVector::OneVector; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
	FVector DestScale = FVector::OneVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Size")
	float Durataion;	// 스케일 값이 변하는 시간 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Life Time")
	float SpawnLifeTime = 1.0f;	// 유지시간 

public:
	UCSkillCollision_Shape();

public:
	FORCEINLINE void SetUpdated(bool InValue) { bUpdated = InValue; }
	FORCEINLINE void SetTargetName(FName InName) { TargetName = InName; }

public:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 충돌 시작
	void ActivateCollision() override;
	// 충돌 종료
	void DeactivateCollision() override;

	void CheckCollision() override;


private:
	void Execute_Scaling(const FVector& InVector);
	void Execute_Laser(const FVector& InVector);

	void EndSkillCollision();


private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnSystemFinished(class UParticleSystemComponent* PSystem);

	UFUNCTION()
	void OnNiagaraSystemFinished(class UNiagaraComponent* PSystem);

protected:
	void DrawDebugCollisionLine() override;


private:
	TArray<AActor*> LastList;

private:
	class UCapsuleComponent* Capsule; 
	class UBoxComponent* Box; 

private:
	float ElapsedTime; 
	FName TargetName = "Default";
	bool bUpdated = false; 
};
