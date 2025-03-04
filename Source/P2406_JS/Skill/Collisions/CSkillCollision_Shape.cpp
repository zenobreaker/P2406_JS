#include "CSkillCollision_Shape.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCSkillCollision_Shape::UCSkillCollision_Shape()
{

}


void UCSkillCollision_Shape::BeginPlay()
{
	Super::BeginPlay();

	Index = 0;

	//ActivateCollision();
}

void UCSkillCollision_Shape::ActivateCollision()
{
	CheckTrue(HitDatas.Num() <= Index);
	CheckNull(OwnerCharacter);

	StartLocation = OwnerCharacter->GetActorLocation();

	if (CollisionData.bRepeat)
	{
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&UCSkillCollisionComponent::CheckCollision,
			CollisionData.CollisionInterval,
			true,
			0.0f);
	}
	else
	{
		CheckCollision();
	}
}

void UCSkillCollision_Shape::DeactivateCollision()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UCSkillCollision_Shape::CheckCollision()
{
	CheckNull(OwnerCharacter);

	if (Index >= HitDatas.Num())
	{
		DeactivateCollision();

		return;
	}

	TArray<FOverlapResult> hitResults;  // ������ ����
	FVector Start = StartLocation;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(CollisionRadius);
	switch (CollisionData.CollisionType)
	{
	case ESkillCollisionType::Sphere:
		CollisionShape = FCollisionShape::MakeSphere(CollisionRadius);
		//DrawDebugSphere(OwnerCharacter->GetWorld(), Start, CollisionRadius, 12, FColor::Red, false, 1.0f);
		break;
	case ESkillCollisionType::Box:
		CollisionShape = FCollisionShape::MakeBox(CollisionData.BoxExtent);
		break;
	case ESkillCollisionType::Capsule:
		CollisionShape = FCollisionShape::MakeCapsule
		(
			CollisionData.CapsuleRadius,
			CollisionData.CapsuleHalfHeight
		);
		break;
	}

	
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
			if (HitActor == nullptr)
				continue;
			if (CheckMyTeam(HitActor) == true)
				continue;

			ACBaseCharacter* character = Cast<ACBaseCharacter>(HitActor);
			if (character == nullptr)
				continue;

			//UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
			HitDatas[Index].SendDamage(OwnerCharacter, nullptr, character);
			if (OnSkillDamageds.Num() > 0)
				OnSkillDamageds[Index].ExecuteIfBound();
			if (OnSkillDamagedOneParams.Num() > 0)
				OnSkillDamagedOneParams[Index].ExecuteIfBound(character);
			if (OnSkillDamagedThreeParams.Num() > 0)
				OnSkillDamagedThreeParams[Index].ExecuteIfBound(OwnerCharacter, nullptr, character);
		}
	}


	Index++;
}

