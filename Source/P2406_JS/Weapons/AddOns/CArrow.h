
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArrowHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestoryer);

UCLASS()
class P2406_JS_API ACArrow : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpan")
	float LifeSpanAfterCollision = 1;

private:
	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* Projectile;

public:
	FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }

public:
	ACArrow();

	void Shoot(const FVector& InForward);

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	FArrowHit OnHit;
	FProjectileEndPlay OnEndPlay;

private:
	TArray<AActor*> Ignores;

};
