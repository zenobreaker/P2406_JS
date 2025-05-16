#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CGrapplingComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGrapplingZoomMode, bool, InVisibility);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGraplingPressed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGraplingReleased);

UENUM()
enum class EGrapplingMode 
{
	Grappling, Hook, Max 
};


UENUM()
enum class EGrapplingState
{
	Aim, Begin, Action, End, Max 
};


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
	UAnimMontage* HookMontage;

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float PlayRate = 1.0f; 

	UPROPERTY(EditAnywhere, Category = "Grappling")
	TSubclassOf<class UUserWidget> TargetUiClass; 

	UPROPERTY(EditAnywhere, Category = "Grappling")
	float TraceDistance = 1000.0f; 

	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

public:
	FORCEINLINE bool GetGrappling() { return GrapplingState == EGrapplingState::Action; }

private:
	UCGrapplingComponent();

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	virtual void PullTowardsTarget();

	void TryGrapplingTarget(); 

	void Pressed(); 
	void Released(); 

private:
	void PullTargetToSelf(AActor* InTarget);

private:
	void SearchGrapplingAbleTarget();
	void DrawGrapplingTargetUI(AActor* InTarget);

public:
	void ResetGrapple();
	void InterruptGrapple();

	void SetTarget();

public:
	void InstallGrapplingRope();
	void UninstallrapplingRope();

public:
	void DoAction(); 
	void DoAction_Grappling();
	void DoAction_Hook();

	void Begin_Grappling(); 
	void End_Grappling(); 

	void Begin_DoGrappling();
	void End_DoGrappling();

	void Begin_Hooking(); 
	void End_Hooking(); 

private:
	void PerformGrapple(float InDeltaTime); 
	void Grapple(AActor* InTarget, float InDetaTime);
	void Grapple_2(float InDetaTime);

public:
	FOnGrapplingZoomMode OnGrapplingZoomMode;
	FOnGraplingPressed OnGraplingPressed;
	FOnGraplingReleased OnGraplingReleased;


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
	bool bGrapplingZoomMode = false; 

private:
	EGrapplingMode GrapMode; 
	EGrapplingState GrapplingState;

private:
	AActor* GrappleTarget;
};
