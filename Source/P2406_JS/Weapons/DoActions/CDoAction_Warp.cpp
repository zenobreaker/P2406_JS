#include "Weapons/DoActions/CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Weapons/CAttachment.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/CStateComponent.h"

UCDoAction_Warp::UCDoAction_Warp()
{
}

void UCDoAction_Warp::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionDatas, InHitDatas);

	Decal = CHelpers::GetComponent<UDecalComponent>(InAttachment);
	PlayerController = InOwner->GetController<APlayerController>();
}

void UCDoAction_Warp::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;

	if (GetCursorLocationAndRotation(location, rotation) == false)
	{
		Decal->SetVisibility(false);

		return;
	}


	Decal->SetVisibility(true);

	Decal->SetWorldLocation(location);
	Decal->SetWorldRotation(rotation);
}

void UCDoAction_Warp::DoAction()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(DoActionDatas.Num() > 0);

	Super::DoAction();


	FRotator rotation;
	if (GetCursorLocationAndRotation(MoveToLocation, rotation))
	{
		float height = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		MoveToLocation.Z += height;

		float yaw = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), MoveToLocation).Yaw;
		OwnerCharacter->SetActorRotation(FRotator(0, yaw, 0));

		DoActionDatas[0].DoAction(OwnerCharacter);
	}
}

void UCDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	OwnerCharacter->SetActorLocation(MoveToLocation);
	MoveToLocation = FVector::ZeroVector;
}

bool UCDoAction_Warp::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	CheckNullResult(PlayerController, false);

	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
	CheckFalseResult(hitResult.bBlockingHit, false);


	OutLocation = hitResult.Location;
	OutRotation = hitResult.ImpactNormal.Rotation();

	return true;
}
