#include "Components/CZoomComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"


UCZoomComponent::UCZoomComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCZoomComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	SpringArm = FHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	CheckNull(SpringArm);

	TargetArmLength = SpringArm->TargetArmLength;
	
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

