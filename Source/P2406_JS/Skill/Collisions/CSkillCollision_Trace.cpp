#include "CSkillCollision_Trace.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Characters/IDamagable.h"
#include "Skill/CSkillEntity.h"

UCSkillCollision_Trace::UCSkillCollision_Trace()
{
	PrimaryComponentTick.bCanEverTick = bDrawDebug; 
}


void UCSkillCollision_Trace::BeginPlay()
{
	Super::BeginPlay();

}

void UCSkillCollision_Trace::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (bDrawDebug && bActivate)
	{
		DrawDebugCollisionLine();
	}
}

void UCSkillCollision_Trace::ActivateCollision()
{
	Super::ActivateCollision();
	
	CheckNull(OwnerCharacter);
	bActivate = true; 

	CheckCollision();
}

void UCSkillCollision_Trace::DeactivateCollision()
{
	Super::DeactivateCollision();

	bActivate = false;
	
}

void UCSkillCollision_Trace::CheckCollision()
{
	Super::CheckCollision();

	CheckNull(OwnerCharacter);

	TArray<FOverlapResult> hitResults;  // ������ ����
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

	
	// Ʈ���̽� ���� 
	FCollisionQueryParams tracePramams;
	tracePramams.AddIgnoredActor(Entity);
	tracePramams.AddIgnoredActor(OwnerCharacter);


	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);


	bool bHit = OwnerCharacter->GetWorld()->OverlapMultiByObjectType
	(
		hitResults,
		start,
		FQuat::Identity,
		ObjectQueryParams,
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

			IIDamagable* damage = Cast<IIDamagable>(HitActor);
			if (damage == nullptr)
				continue;
			HitData.SendDamage(OwnerCharacter, Entity, HitActor);
		}
	}
}

void UCSkillCollision_Trace::DrawDebugCollisionLine()
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

