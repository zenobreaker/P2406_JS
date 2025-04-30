#include "Skill/Entities/CSkillEntity_Decal.h"
#include "Global.h"
#include "GameFramework/PlayerController.h"
#include "Components/DecalComponent.h"


ACSkillEntity_Decal::ACSkillEntity_Decal()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACSkillEntity_Decal::BeginPlay()
{
	Super::BeginPlay(); 

	CheckNull(OwnerCharacter); 

	PlayerController = OwnerCharacter->GetController<APlayerController>(); 
	Decal = FHelpers::GetComponent<UDecalComponent>(this);
	/*if(Decal!= nullptr)
		Decal->SetVisibility(false);*/
}

void ACSkillEntity_Decal::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;

	if (GetCursorLocationAndRotation(location, rotation) == false)
		return;

	CheckNull(Decal);
	Decal->SetWorldLocation(location);
	Decal->SetWorldRotation(rotation);
}

void ACSkillEntity_Decal::SetVisibleDecal(bool InValue)
{
	CheckNull(Decal);

	Decal->SetVisibility(InValue);
}

void ACSkillEntity_Decal::GetCapturedLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	CheckNull(PlayerController);

	FHitResult hitResult;
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
	CheckFalse(hitResult.bBlockingHit);

	OutLocation = hitResult.Location;
	OutRotation = hitResult.ImpactNormal.Rotation();
}

bool ACSkillEntity_Decal::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	CheckNullResult(PlayerController, false); 

	FHitResult hitResult; 
	PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);
	CheckFalseResult(hitResult.bBlockingHit, false); 

	OutLocation = hitResult.Location;
	OutRotation = hitResult.ImpactNormal.Rotation();
	
	return true;
}
