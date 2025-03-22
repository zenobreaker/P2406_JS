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

		// 레이저 타입이면 캡슐을 회전시킴
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
			Capsule->RegisterComponent();  // 컴포넌트 등록

		
		}
		else
		{
			Box->bHiddenInGame = false;
			Box->SetVisibility(true);
			Box->RegisterComponent();  // 컴포넌트 등록

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

// 레이저타입은 시전위치에서부터 점점 X축이 길어진다. 
void UCSkillCollision_Scaling::Execute_Laser(const FVector& InVector)
{
	CheckNull(Entity); 

	
	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
	{
		// 한 면은 캐릭터 기준으로 고정시키고, 나머지 면은 앞으로 나가도록 설정
		FVector up = Capsule->GetUpVector();
		FVector startLocation = Entity->GetActorLocation();
		//X 값으로만 계산 캡슐은 중심으로 움직이므로 전반 나눔
		FVector endLocation = startLocation + up * (InVector.X * 0.5f);

		// 레이저 타입은 값의 타입이 반대 
		Capsule->SetCapsuleSize(InVector.Y, InVector.X);
		Capsule->SetWorldLocation(endLocation);
	}
	else if (CollisionData.CollisionType == ESkillCollisionType::Box)
	{
		// 한 면은 캐릭터 기준으로 고정시키고, 나머지 면은 앞으로 나가도록 설정
		FVector forward = Entity->GetActorForwardVector();
		FVector startLocation = Entity->GetActorLocation();
		//X 값으로만 계산 
		FVector endLocation = startLocation + forward * InVector.X;

		Box->SetBoxExtent(InVector);
		Box->SetWorldLocation(endLocation); 
	}
}


void UCSkillCollision_Scaling::DrawDebugCollisionLine()
{
}

