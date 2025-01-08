#include "Weapons/SubActions/CSubAction_Bow.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Weapons/Attachments/CAttachment_Bow.h"

UCSubAction_Bow::UCSubAction_Bow()
{
	//FHelpers::GetAsset<UCurveVector>(&Curve, "/Script/Engine.CurveVector'/Game/Weapons/Bow/Curve_Aiming.Curve_Aiming'");
}

void UCSubAction_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	SpringArm = FHelpers::GetComponent<USpringArmComponent>(InOwner);
	Camera = FHelpers::GetComponent<UCameraComponent>(InOwner); 

	FOnTimelineVector timeline; 
	timeline.BindUFunction(this, "OnAiming");

	Timeline.AddInterpVector(Curve, timeline);
	Timeline.SetPlayRate(AimingSpeed);

	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);

	if (!!bow)
		Bending = bow->GetAniminstance_Bending();
}

void UCSubAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	Timeline.TickTimeline(InDeltaTime);
}

void UCSubAction_Bow::Pressed()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Pressed();
	State->OnSubActionMode();

	// 현재 값 미리 저장해놓기 
	OriginData.TargetArmLength = SpringArm->TargetArmLength;
	OriginData.SocketOffset = SpringArm->SocketOffset;
	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	// 최신 값으로 세팅 
	SpringArm->TargetArmLength = AimData.TargetArmLength;
	SpringArm->SocketOffset = AimData.SocketOffset;
	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	//Camera->FieldOfView = 45;

	Timeline.PlayFromStart();
}


void UCSubAction_Bow::Released()
{
	CheckNull(SpringArm);
	CheckNull(Camera);

	Super::Released();

	State->OffSubActionMode();
	
	SpringArm->TargetArmLength = OriginData.TargetArmLength;
	SpringArm->SocketOffset = OriginData.SocketOffset;
	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);

	//Camera->FieldOfView = 90;

	Timeline.ReverseFromEnd();
}



void UCSubAction_Bow::OnAiming(FVector Output)
{
	Camera->FieldOfView = Output.X;

	if (!!Bending)
		*Bending = Output.Y;
}
