#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"


UCLASS( )
class P2406_JS_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Settings")
	class UParticleSystem* ParticleAsset; 

	UPROPERTY(EditAnywhere, Category = "Settings")
	float TraceDistance = 1000.0f; 

	UPROPERTY(EditAnywhere, Category = "Settings")
	float InterpSpeed = 5.0f; 

	UPROPERTY(EditAnywhere, Category = "Settings")
	float FinishAngle = 2.0f; 

public:	
	UCTargetComponent();

public:
	FORCEINLINE bool GetTargetMode() { return Target == nullptr; }

protected:
	virtual void BeginPlay() override;

public:
	void Toggle();
	void MoveLeft(); 
	void MoveRight(); 

private:
	void Start();
	void End(); 

private:
	void Change(class ACharacter* InCandidate);
	void Move(bool bRight);

private:
	class ACharacter* GetNearlyFrontAngle(const TArray<FHitResult>& InHitResults);

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ACharacter* OwnerCharacter;
	class ACharacter* Target;

	class UParticleSystemComponent* Particle;

private:
	bool bMovingFocus; 
		
};
