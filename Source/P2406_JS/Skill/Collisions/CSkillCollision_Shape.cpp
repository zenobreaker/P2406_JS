#include "CSkillCollision_Shape.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Skill/CSkillEntity.h"

UCSkillCollision_Shape::UCSkillCollision_Shape()
{
	PrimaryComponentTick.bCanEverTick = true; 
}


void UCSkillCollision_Shape::BeginPlay()
{
	Super::BeginPlay();

}

void UCSkillCollision_Shape::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bDrawDebug && bActivate)
	{
		DrawDebugCollisionLine();
	}
}

void UCSkillCollision_Shape::ActivateCollision(int32 InIndex)
{
	Super::ActivateCollision(InIndex);
	Index = InIndex;

	CheckTrue(HitDatas.Num() <= Index);
	CheckNull(OwnerCharacter);
	bActivate = true; 
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

void UCSkillCollision_Shape::DeactivateCollision(int32 InIndex)
{
	Super::DeactivateCollision(InIndex);

	bActivate = false;
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UCSkillCollision_Shape::CheckCollision()
{
	Super::CheckCollision();

	CheckNull(OwnerCharacter);

	if (Index >= HitDatas.Num())
	{
		DeactivateCollision();

		return;
	}

	TArray<FOverlapResult> hitResults;  // 변수명 수정
	FVector start = GetComponentLocation();

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(CollisionRadius);
	switch (CollisionData.CollisionType)
	{
	case ESkillCollisionType::Sphere:
		CollisionShape = FCollisionShape::MakeSphere(CollisionRadius);
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

	DrawDebugCollisionLine();

	
	// 트레이스 설정 
	FCollisionQueryParams tracePramams;
	tracePramams.AddIgnoredActor(Entity);
	tracePramams.AddIgnoredActor(OwnerCharacter);

	bool bHit = OwnerCharacter->GetWorld()->OverlapMultiByObjectType
	(
		hitResults,
		start,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		CollisionShape,
		tracePramams
	);

	if (bHit)
	{
		for (auto& Hit : hitResults)
		{
			if (Hit.GetActor() == nullptr)
				continue;
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

void UCSkillCollision_Shape::DrawDebugCollisionLine()
{
	CheckFalse(bDrawDebug);
	CheckNull(OwnerCharacter);

	FVector start = GetComponentLocation();
	switch (CollisionData.CollisionType)
	{
	case ESkillCollisionType::Sphere:
		DrawDebugSphere(OwnerCharacter->GetWorld(), start, CollisionRadius, 12, FColor::Red, false, 1.0f);
		break;
	case ESkillCollisionType::Box:
		///DrawDebugBox(OwnerCharacter->GetWorld(), )
		break;
	case ESkillCollisionType::Capsule:

		break;
	}
}

