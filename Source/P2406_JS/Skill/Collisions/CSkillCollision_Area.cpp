#include "CSkillCollision_Area.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCSkillCollision_Area::UCSkillCollision_Area()
{

}


void UCSkillCollision_Area::BeginPlay()
{
	Super::BeginPlay();

	Index = 0;
}

void UCSkillCollision_Area::ActivateCollision()
{
	CheckTrue(HitDatas.Num() <= Index);
	CheckNull(OwnerCharacter);

	StartLocation = OwnerCharacter->GetActorLocation();

	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		this,
		&UCSkillCollisionComponent::CheckCollision,
		CollisionData.CollisionInterval,
		true);
}

void UCSkillCollision_Area::DeactivateCollision()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UCSkillCollision_Area::CheckCollision()
{
	CheckNull(OwnerCharacter);

	TArray<FOverlapResult> hitResults;  // ������ ����
	FVector Start = StartLocation;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(CollisionRadius);
	DrawDebugSphere(OwnerCharacter->GetWorld(), Start, CollisionRadius, 12, FColor::Red, false, 1.0f);

	// Ʈ���̽� ���� 
	FCollisionQueryParams tracePramams;
	tracePramams.AddIgnoredActor(OwnerCharacter);

	bool bHit = OwnerCharacter->GetWorld()->OverlapMultiByObjectType
	(
		hitResults,
		Start, 
		FQuat::Identity, 
		FCollisionObjectQueryParams(ECC_Pawn),
		CollisionShape,
		tracePramams
	);

	if (bHit)
	{
		for (auto& Hit : hitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			}
		}
	}

}

