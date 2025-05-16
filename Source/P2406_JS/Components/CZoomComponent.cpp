#include "Components/CZoomComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Curves/CurveVector.h"
#include "Camera/CameraComponent.h"


UCZoomComponent::UCZoomComponent()
{
	FHelpers::GetAsset<UCurveVector>(&Curve,
		"/Script/Engine.CurveVector'/Game/Parkour/Zoom_Aiming.Zoom_Aiming'");

	PrimaryComponentTick.bCanEverTick = true;
}


void UCZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	Camera = FHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	SpringArm = FHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	CheckNull(SpringArm);

	TargetArmLength = SpringArm->TargetArmLength;

	FOnTimelineVector timeline;
	timeline.BindUFunction(this, "OnZoomAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(ZoomAimingSpeed);

	SetOriginCameraData();
}


void UCZoomComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckNull(SpringArm);
	CheckFalse(bZoomable);
	CheckTrue(FMath::IsNearlyEqual(SpringArm->TargetArmLength, TargetArmLength, 1e-6f));

	SpringArm->TargetArmLength = UKismetMathLibrary::FInterpTo(SpringArm->TargetArmLength, TargetArmLength, DeltaTime, InterpSpeed);
}

void UCZoomComponent::SetValue(float InZoomValue)
{
	TargetArmLength += (Speed * InZoomValue);
	TargetArmLength = FMath::Clamp(TargetArmLength, Range.X, Range.Y);
}

void UCZoomComponent::OnZoomPressed()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	SetOriginCameraData();

	// 최신 값으로 세팅 
	SpringArm->TargetArmLength = ZoomAimData.TargetArmLength;
	SpringArm->SocketOffset = ZoomAimData.SocketOffset;
	SpringArm->bEnableCameraLag = ZoomAimData.bEnableCameraLag;
	Camera->SetRelativeLocation(ZoomAimData.CameraLocation);

	Timeline.PlayFromStart();

	bGrapplingAiming = true;

	SetZoomOff();
}

void UCZoomComponent::OnZoomReleased()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	//Timeline.ReverseFromEnd();

	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	bGrapplingAiming = false;

	SetZoomOn();
}

void UCZoomComponent::OnZoomAiming(FVector Output)
{
	CheckNull(Camera);

	Camera->FieldOfView = Output.X;
}

void UCZoomComponent::SetOriginCameraData()
{
	CheckNull(SpringArm);


	// 현재 값 미리 저장해놓기 
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();
}

