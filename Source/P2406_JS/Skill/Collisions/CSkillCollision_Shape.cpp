#include "Skill/Collisions/CSkillCollision_Shape.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Skill/CSkillEntity.h"
#include "Characters/IDamagable.h"

UCSkillCollision_Shape::UCSkillCollision_Shape()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Capsule->InitCapsuleSize(InitScale.X, InitScale.Y);

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	Box->InitBoxExtent(InitScale);

	PrimaryComponentTick.bCanEverTick = true;
}

void UCSkillCollision_Shape::BeginPlay()
{
	Super::BeginPlay();

	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
	{
		//Capsule->AttachToComponent(Entity->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
		Capsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		Capsule->OnComponentBeginOverlap.AddDynamic(this, &UCSkillCollision_Shape::OnComponentBeginOverlap);
		Capsule->OnComponentEndOverlap.AddDynamic(this, &UCSkillCollision_Shape::OnComponentEndOverlap);
		Capsule->SetWorldLocation(Entity->GetActorLocation());
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
		Box->OnComponentBeginOverlap.AddDynamic(this, &UCSkillCollision_Shape::OnComponentBeginOverlap);
		Box->OnComponentEndOverlap.AddDynamic(this, &UCSkillCollision_Shape::OnComponentEndOverlap);

		Box->SetWorldLocation(Entity->GetActorLocation());
		Box->SetWorldRotation(Entity->GetActorRotation());
	}

	if (!!Particle)
	{
		Particle->OnSystemFinished.AddDynamic(this, &UCSkillCollision_Shape::OnSystemFinished);
		FLog::Log("Particle : " + Particle->GetComponentScale().ToString());
	}
	if (!!Niagara)
		Niagara->OnSystemFinished.AddDynamic(this, &UCSkillCollision_Shape::OnNiagaraSystemFinished);
}


void UCSkillCollision_Shape::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	CheckFalse(bActivate);
	if (SpawnLifeTime >= -1.0f && ElapsedTime >= SpawnLifeTime)
	{
		DeactivateCollision();
		return; 
	}
	CheckTrue(Type == EScaleType::Max);

	float alpha = FMath::Clamp(ElapsedTime / Durataion, 0.0f, 1.0f);

	FVector currentScale = FMath::Lerp(InitScale, DestScale, alpha);

	if (Type == EScaleType::AOE)
		Execute_Scaling(currentScale);
	else if (Type == EScaleType::Laser_Axis_X)
		Execute_Laser(currentScale);

	ElapsedTime += DeltaTime;
	//FLog::Print("Scaling : " + currentScale.ToString() + " " + FString::SanitizeFloat(ElapsedTime), 7535);

	
	CheckFalse(bUpdated);

	USkeletalMeshComponent* skeletal = FHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);
	CheckNull(skeletal);

	if (TargetName.IsEqual("Default"))
		return; 
	FTransform transform = skeletal->GetSocketTransform(TargetName);
	Capsule->SetWorldLocationAndRotation(transform.GetLocation(),
		transform.GetRotation());
	Box->SetWorldLocationAndRotation(transform.GetLocation(),
		transform.GetRotation());
}

void UCSkillCollision_Shape::ActivateCollision()
{
	Super::ActivateCollision();
	CheckNull(OwnerCharacter);

	ElapsedTime = 0.0f;

	bActivate = true;
	
	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
	{
		Capsule->bHiddenInGame = false;
		Capsule->SetVisibility(bDrawDebug);
		Capsule->RegisterComponent();  // 컴포넌트 등록
	}
	else
	{
		Box->bHiddenInGame = false;
		Box->SetVisibility(bDrawDebug);
		Box->RegisterComponent();  // 컴포넌트 등록
	}
	

	if (CollisionData.bRepeat)
	{
		FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]()
			{
				CheckCollision();
			});

		GetWorld()->GetTimerManager().SetTimer(TimerHandle,
			timerDelegate,
			CollisionData.CollisionInterval,
			true, 0);
	}
}


void UCSkillCollision_Shape::DeactivateCollision()
{
	Super::DeactivateCollision();

	EndSkillCollision();
}

void UCSkillCollision_Shape::CheckCollision()
{
	CheckFalse(Hitted.Num() > 0);

	for (int i = Hitted.Num() - 1; i >= 0; i--)
	{
		if (CollisionData.bRepeat == false)
		{
			if (LastList.Find(Hitted[i]) != INDEX_NONE)
				continue;
		}

		FLog::Log(" Hit! : " + Hitted[i]->GetName());
		HitData.SendDamage(OwnerCharacter, Entity, Hitted[i]);

		/*if(Hitted[i] != nullptr)
			LastList.AddUnique(Hitted[i]);*/
	}

	//TODO : 엔티티에 일부 기능을 넣어놓기
	// 데미지 데이터 개수만큼 트레이싱발사
	// 

	//if (CollisionData.bRepeat == false && HitDatas.Num() >= Index)
	//	Index++;
}



void UCSkillCollision_Shape::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == OwnerCharacter);
	CheckTrue(CheckMyTeam(OtherActor));
	CheckNull(Cast<IIDamagable>(OtherActor));

	Hitted.AddUnique(OtherActor);

	CheckTrue(CollisionData.bRepeat);
	CheckCollision();
}

void UCSkillCollision_Shape::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OtherActor == OwnerCharacter);

	CheckFalse(Hitted.Num() > 0);

	Hitted.Remove(OtherActor);
}

void UCSkillCollision_Shape::OnSystemFinished(UParticleSystemComponent* PSystem)
{
	EndSkillCollision();
}

void UCSkillCollision_Shape::OnNiagaraSystemFinished(UNiagaraComponent* PSystem)
{
	EndSkillCollision();
}


void UCSkillCollision_Shape::Execute_Scaling(const FVector& InVector)
{
	if (CollisionData.CollisionType == ESkillCollisionType::Capsule || CollisionData.CollisionType == ESkillCollisionType::Sphere)
		Capsule->SetCapsuleSize(InVector.X, InVector.Y);
	else if (CollisionData.CollisionType == ESkillCollisionType::Box)
		Box->SetBoxExtent(InVector);
}

// 레이저타입은 시전위치에서부터 점점 X축이 길어진다. 
void UCSkillCollision_Shape::Execute_Laser(const FVector& InVector)
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

void UCSkillCollision_Shape::EndSkillCollision()
{
	bActivate = false;

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Capsule->DestroyComponent();
	Box->DestroyComponent();
	LastList.Empty();
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}


void UCSkillCollision_Shape::DrawDebugCollisionLine()
{
}
