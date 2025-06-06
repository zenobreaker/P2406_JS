#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDashComponent.generated.h"

UENUM(BlueprintType)
enum class DashDirection : uint8
{
	Forward = 0, Back, Left, Right, MAX = Right,
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCDashComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Dash")
	TArray<class UAnimMontage*>  DashMontages;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashSpeed = 1000;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float BlurAmount = 1.5f;

	UPROPERTY(EditAnywhere, Category = "Dash")
	class USoundWave* Sound;

	UPROPERTY(EditAnywhere, Category = "Trail")
	TSubclassOf<class ACGhostTrail> GhostTrailClass;

	UPROPERTY(EditAnywhere, Category = "Trail")
	int32 TrailCount = 50;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug = false; 


public:
	UCDashComponent();

protected:
	void BeginPlay() override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void OnDash();
	void DashAction();
	bool IsEvade();

public:
	void Begin_DashSpeed();
	void End_DashSpeed();

public:
	void CreateEvadeOverlap(const FVector& InPrevLocation);
	void DestroyEvadeOverlap();

private:
	void PlaySoundWave();
	void CheckEvadeHit();

private:
	void Destroy_SingleGhostTrail();

private:
	FVector GetGroundNormal();

private:
	void HandleBeginDash();
	void HandleEndDash();
	void HandleEvade();

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

private:
	ACharacter* OwnerCharacter;
	class UCMovementComponent* Movement;
	class UCTargetComponent* target;
	class UCWeaponComponent* Weapon;
	class UCStateComponent* State;
	class UCameraComponent* Camera;

private:
	class USphereComponent* DashAvoidanceCollision;

private:
	TArray<class ACGhostTrail*> GhostTrails;


private:
	bool bTargeting = false;
	bool bIsDashing = false;
	bool bIsEvadeSuccessed = false;
	const bool* bControlRotation;
	const bool* bCanMove; 

	FVector* InputVec;
	int32 CurrentTrailCount = 0;
	float StartDashTime = 0.0f;
	FVector PrevLocation;
	FVector GroundNormal;

private:
	DashDirection DashDir;

private:
	FTimerHandle DestroyTimer;
	FTimerHandle SpawnTimer;
};
