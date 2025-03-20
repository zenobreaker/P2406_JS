#include "Skill/Collisions/CSkillCollision_Scaling.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "Skill/CSkillEntity.h"


UCSkillCollision_Scaling::UCSkillCollision_Scaling()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("");
	Capsule->InitCapsuleSize(InitScale.X, InitScale.Y);
	Capsule->SetupAttachment(this);
	
	Box = CreateDefaultSubobject<UBoxComponent>("");
	Box->InitBoxExtent(InitScale);
	Box->SetupAttachment(this);
}

void UCSkillCollision_Scaling::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &UCSkillCollision_Scaling::OnComponentBeginOverlap);
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else if (CollisionData.CollisionType == ESkillCollisionType::Box)
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->OnComponentBeginOverlap.AddDynamic(this, &UCSkillCollision_Scaling::OnComponentBeginOverlap);
	}
}

void UCSkillCollision_Scaling::ActivateCollision(int32 InIndex)
{
	Super::ActivateCollision(InIndex);
	CheckTrue(HitDatas.Num() <= Index);
	CheckNull(OwnerCharacter);
	
	Index = InIndex;
	
	ElapsedTime = 0.0f;

	bActivate = true; 
}

void UCSkillCollision_Scaling::DeactivateCollision(int32 InIndex)
{
	Super::DeactivateCollision(InIndex);

	bActivate = false;

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UCSkillCollision_Scaling::CheckCollision()
{

}

void UCSkillCollision_Scaling::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}

void UCSkillCollision_Scaling::DrawDebugCollisionLine()
{
}

void UCSkillCollision_Scaling::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(bActivate);

	float alpha = FMath::Clamp(ElapsedTime / Durataion, 0.0f, 1.0f);

	CurrentScale = FMath::Lerp(InitScale, DestScale, alpha);

	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
		Capsule->SetCapsuleSize(CurrentScale.X, CurrentScale.Y);
	else
		Box->SetBoxExtent(CurrentScale);
	
	ElapsedTime += DeltaTime;
}
