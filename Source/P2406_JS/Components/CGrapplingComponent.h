#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CGrapplingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrapplingZoomMode, bool, InVisibility);

UCLASS()
class P2406_JS_API UCGrapplingComponent : public UActorComponent
{
	GENERATED_BODY()


private:
	//UPROPERTY(EditAnywhere, Category = "Grappling")
	//class UCableComponent* GrappleCable;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float PullSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	UAnimMontage* GrapplingMontage;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float PlayRate = 1.0f; 

	UPROPERTY(EditAnywhere, Category = "Grappling")
	TSubclassOf<class UUserWidget> TargetUiClass; 

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float TraceDistance = 1000.0f; 

	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

public:
	FORCEINLINE bool GetGrappling() { return bIsGrappling; }

private:
	UCGrapplingComponent();

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void OnGrapple();
	
	virtual void PullTowardsTarget();

	void TryGrapplingTarget(); 

private: 
	void SearchGrapplingAbleTarget();
	void DrawGrapplingTargetUI(AActor* InTarget);

public:
	void OnGrappling_Pressed();
	void OnGrappling_Released();

	void ResetGrapple();
	void InterruptGrapple();

	void SetTarget();
public:
	void InstallrapplingRope(); 
	void UninstallrapplingRope(); 

	void Begin_DoGrappling();
	void End_DoGrappling();


private:
	void Grapple_1(float InDetaTime);
	void Grapple_2(float InDetaTime);

public:
	FOnGrapplingZoomMode OnGrapplingZoomMode;

private:
	class ACharacter* OwnerCharacter;
	class UCableComponent* Cable; 
	class UCZoomComponent* Zoom; 
	class UCTargetComponent* Target; 
	class UWidgetComponent* TargetUi;

private:
	FVector TargetLocation;

	float MaxLineDistance = 1600.0f;
	float DistanceThreshold = 100.0f;
	bool bIsGrappling = false;
	bool bGrappleEnd = false;
	bool bGrapplingZoomMode = false; 

private:
	AActor* GrappleTarget;
};
