#include "Components/CGrapplingComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCGrapplingComponent::UCGrapplingComponent()
{

}


void UCGrapplingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCGrapplingComponent::OnGrapple()
{
	CheckNull(OwnerCharacter);
	CheckTrue(isGrappling);

	isGrappling = true; 

	FHitResult HitResult;
	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = Start + (OwnerCharacter->GetActorForwardVector() * MaxLineDistance);  // 원하는 거리로 트레이스
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		TargetLocation = HitResult.Location;
		// 여기서 타겟 위치를 설정
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, true, 5.0f);
	}
	else
	{
		CLog::Print("No Colloisions!");
		DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, true, 5.0f);
	}

}

void UCGrapplingComponent::PullTowardsTarget()
{
}

void UCGrapplingComponent::ResetGrapple()
{
	isGrappling = false;
}



