#include "Components/CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

//#define LOG_UCFeetComponent


UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	float leftDistance;
	FRotator leftRotation;

	Trace(LeftBoneName, leftDistance, leftRotation);

	float rightDistance;
	FRotator rightRotation;

	Trace(RightBoneName, rightDistance, rightRotation);


	float pelvisDistance = FMath::Min(leftDistance, rightDistance);
	Data.PelvisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PelvisDistance.Z, pelvisDistance, DeltaTime, InterpSpeed);

	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (leftDistance - pelvisDistance), DeltaTime, InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(rightDistance - pelvisDistance), DeltaTime, InterpSpeed);

#ifdef LOG_UCFeetComponent
	FLog::Print(Data.LeftDistance, 9990);
	FLog::Print(Data.RightDistance, 9991);
	FLog::Print(Data.PelvisDistance, 9992);
#endif
}

void UCFeetComponent::Trace(FName InName, float& OutDistance, FRotator& OutRotation)
{
	FVector  location = OwnerCharacter->GetMesh()->GetSocketLocation(InName);

	float z = OwnerCharacter->GetActorLocation().Z;
	FVector start = FVector(location.X, location.Y, z);

	z = z - OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - TraceDistance;
	FVector end = FVector(location.X, location.Y, z);

	//DrawDebugLine(OwnerCharacter->GetWorld(), start, end, FColor::Green);

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	FHitResult hitResult;
	UKismetSystemLibrary::LineTraceSingle(
		GetWorld(),
		start,
		end,
		ETraceTypeQuery::TraceTypeQuery1,
		true,
		ignores,
		DrawDebug,
		hitResult,
		true);

	OutDistance = 0.0f;
	OutRotation = FRotator::ZeroRotator;

	CheckFalse(hitResult.bBlockingHit);


	float length = (hitResult.ImpactPoint - hitResult.TraceEnd).Size();
	OutDistance = length + OffsetDistance - TraceDistance;


	float roll = UKismetMathLibrary::DegAtan2(hitResult.Normal.Y, hitResult.Normal.Z);
	float pitch = -UKismetMathLibrary::DegAtan2(hitResult.Normal.X, hitResult.Normal.Z);

	OutRotation = FRotator(pitch, 0.0f, roll);
}

