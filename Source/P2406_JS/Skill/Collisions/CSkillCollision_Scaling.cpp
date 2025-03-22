#include "Skill/Collisions/CSkillCollision_Scaling.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"

#include "Skill/CSkillEntity.h"


UCSkillCollision_Scaling::UCSkillCollision_Scaling()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->InitCapsuleSize(InitScale.X, InitScale.Y);
	

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->InitBoxExtent(InitScale);
//	Box->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

	PrimaryComponentTick.bCanEverTick = true;
}

void UCSkillCollision_Scaling::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
	{
		//Capsule->AttachToComponent(Entity->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &UCSkillCollision_Scaling::OnComponentBeginOverlap);
	
		Capsule->SetWorldRotation(Entity->GetActorRotation());

		// ������ Ÿ���̸� ĸ���� ȸ����Ŵ
		if (Type == EScaleType::Laser_Axis_X)
		{
			Capsule->SetWorldRotation(FRotator(-90, Entity->GetActorRotation().Yaw, Entity->GetActorRotation().Roll));
		}
		
		Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else if (CollisionData.CollisionType == ESkillCollisionType::Box)
	{
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Box->OnComponentBeginOverlap.AddDynamic(this, &UCSkillCollision_Scaling::OnComponentBeginOverlap);

		Box->SetWorldRotation(Entity->GetActorRotation());
	}
}


void UCSkillCollision_Scaling::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(bActivate);
	CheckTrue(ElapsedTime >= SpawnLifeTime);
	CheckTrue(Type == EScaleType::Max);

	float alpha = FMath::Clamp(ElapsedTime / Durataion, 0.0f, 1.0f);

	FVector currentScale = FMath::Lerp(InitScale, DestScale, alpha);

	if (Type == EScaleType::AOE)
		Execute_Scaling(currentScale);
	else if (Type == EScaleType::Laser_Axis_X)
		Execute_Laser(currentScale);
	
	ElapsedTime += DeltaTime;
	FLog::Print("Scaling : " + currentScale.ToString() + " " + FString::SanitizeFloat(ElapsedTime), 7535);
}


void UCSkillCollision_Scaling::ActivateCollision(int32 InIndex)
{
	Super::ActivateCollision(InIndex);
	CheckTrue(HitDatas.Num() <= Index);
	CheckNull(OwnerCharacter);

	Index = InIndex;

	ElapsedTime = 0.0f;

	bActivate = true;

	if (bDrawDebug)
	{
		if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
		{
			Capsule->bHiddenInGame = false;
			Capsule->SetVisibility(true);
			Capsule->RegisterComponent();  // ������Ʈ ���

		
		}
		else
		{
			Box->bHiddenInGame = false;
			Box->SetVisibility(true);
			Box->RegisterComponent();  // ������Ʈ ���

		}
	}

	FTimerDelegate timerDelegate;
	/*GetWorld()->GetTimerManager().SetTimer(
	TimerHandle,
	this, SpawnLifeTime, timerDelegate);*/
}

void UCSkillCollision_Scaling::DeactivateCollision(int32 InIndex)
{
	Super::DeactivateCollision(InIndex);

	bActivate = false;

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Capsule->DestroyComponent();
	Box->DestroyComponent();
}

void UCSkillCollision_Scaling::CheckCollision()
{

}



void UCSkillCollision_Scaling::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}


void UCSkillCollision_Scaling::Execute_Scaling(const FVector& InVector)
{
	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
		Capsule->SetCapsuleSize(InVector.X, InVector.Y);
	else if (CollisionData.CollisionType == ESkillCollisionType::Box)
		Box->SetBoxExtent(InVector);
}

// ������Ÿ���� ������ġ�������� ���� X���� �������. 
void UCSkillCollision_Scaling::Execute_Laser(const FVector& InVector)
{
	CheckNull(Entity); 

	
	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
	{
		// �� ���� ĳ���� �������� ������Ű��, ������ ���� ������ �������� ����
		FVector up = Capsule->GetUpVector();
		FVector startLocation = Entity->GetActorLocation();
		//X �����θ� ��� ĸ���� �߽����� �����̹Ƿ� ���� ����
		FVector endLocation = startLocation + up * (InVector.X * 0.5f);

		// ������ Ÿ���� ���� Ÿ���� �ݴ� 
		Capsule->SetCapsuleSize(InVector.Y, InVector.X);
		Capsule->SetWorldLocation(endLocation);
	}
	else if (CollisionData.CollisionType == ESkillCollisionType::Box)
	{
		// �� ���� ĳ���� �������� ������Ű��, ������ ���� ������ �������� ����
		FVector forward = Entity->GetActorForwardVector();
		FVector startLocation = Entity->GetActorLocation();
		//X �����θ� ��� 
		FVector endLocation = startLocation + forward * InVector.X;

		Box->SetBoxExtent(InVector);
		Box->SetWorldLocation(endLocation); 
	}
}


void UCSkillCollision_Scaling::DrawDebugCollisionLine()
{
}

