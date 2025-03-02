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

void UCSkillCollision_Area::DeactivateCollision()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void UCSkillCollision_Area::CheckCollision()
{
	CheckNull(OwnerCharacter);

	if (Index >= HitDatas.Num())
	{
		DeactivateCollision();

		return;
	}

	TArray<FOverlapResult> hitResults;  // 변수명 수정
	FVector Start = StartLocation;

	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(CollisionRadius);
	//DrawDebugSphere(OwnerCharacter->GetWorld(), Start, CollisionRadius, 12, FColor::Red, false, 1.0f);

	// 트레이스 설정 
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

