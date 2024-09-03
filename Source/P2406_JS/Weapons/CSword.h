#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSword.generated.h"

UCLASS()
class P2406_JS_API ACSword : public AActor
{
	GENERATED_BODY()
	
private:
	UPROPERTY(VisibleAnywhere)
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere)	
	class UCapsuleComponent* Capsule;


public:	
	ACSword();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
