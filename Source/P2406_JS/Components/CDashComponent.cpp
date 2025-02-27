#include "Components/CDashComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/CGhostTrail.h"
#include "Camera/CameraComponent.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CWeaponComponent.h"
#include <Components/SphereComponent.h>
#include "Components/CapsuleComponent.h"

#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/AddOns/AttackInterface.h"

//#define  LOG_UCDashComponent


UCDashComponent::UCDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCDashComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!!OwnerCharacter)
	{
		Movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
		target = FHelpers::GetComponent<UCTargetComponent>(OwnerCharacter);
		Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
		State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		Camera = FHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	}

	REGISTER_EVENT_WITH_REPLACE(State, OnStateTypeChanged, this, UCDashComponent::OnStateTypeChanged);

	if (Movement != nullptr)
	{
		bCanMove = Movement->GetCanMovePtr();
		bControlRotation = Movement->GetCanControlRotaionPtr();
	}
}

void UCDashComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsDashing == true)
	{
		//PrevLocation = FMath::VInterpTo(PrevLocation, OwnerCharacter->GetActorLocation(), DeltaTime, 5.0f);

		CheckEvadeHit();
	}
}

void UCDashComponent::OnDash()
{
	CheckNull(Movement);

	DashAction();
}

void UCDashComponent::DashAction()
{
	//CheckTrue(State->IsDashMode());
	CheckNull(bCanMove);
	CheckFalse(*bCanMove);
	CheckTrue(bIsDashing);
	CheckNull(OwnerCharacter);

	// 콜한 시점의 위치 저장 
	PrevLocation = OwnerCharacter->GetActorLocation();
	FVector right = OwnerCharacter->GetActorRightVector();
//	FVector move = OwnerCharacter->GetVelocity().GetSafeNormal();
	FVector moveInput = OwnerCharacter->GetLastMovementInputVector().GetSafeNormal();

	GroundNormal = GetGroundNormal();


	float dot = FVector::DotProduct(right, moveInput);

	// 자유 카메라 모드 일 때 
	if (bControlRotation == nullptr || *bControlRotation == false)
	{
		CheckTrue(DashMontages.Num() == 0);
		// 전방
		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Forward]);
	}
	// 고정 회전 모드 일때
	else
	{
		// 전방
		DashDirection dir = DashDirection::Forward;

		const float Magrin = 0.3f;
		if (dot > Magrin)
			dir = DashDirection::Right;
		else if (dot < -Magrin)
			dir = DashDirection::Left;
		else if (moveInput.X <= 0)
			dir = DashDirection::Back;


		CheckTrue(DashMontages.Num() == 0);

		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)dir]);
	}
}

void UCDashComponent::Begin_DashSpeed()
{
	CheckNull(OwnerCharacter);

	HandleBeginDash();

	FVector dashDir = FVector::ZeroVector;
	double xAxis = 1.0f;

	if (OwnerCharacter->GetLastMovementInputVector().IsNearlyZero())
	{
		FRotator rotator = FRotator(0, OwnerCharacter->GetActorRotation().Yaw, 0);
		FVector dir_Start = OwnerCharacter->GetActorLocation();
		FVector dir_End = dir_Start + FQuat(rotator).GetForwardVector() * 200.0f;

		FVector dir = dir_End - dir_Start;
		dir = dir.GetSafeNormal() * -1.0f;
		dashDir = dir;
	}
	else
		dashDir = OwnerCharacter->GetLastMovementInputVector();

	FVector adjustDashDir = dashDir - FVector::DotProduct(dashDir, GroundNormal) * GroundNormal;

	// Dash 
	OwnerCharacter->LaunchCharacter(adjustDashDir * DashSpeed, true, true);
}

void UCDashComponent::End_DashSpeed()
{
	//Movement->SetSpeed(ESpeedType::Run);
	GroundNormal = FVector::ZeroVector;

	HandleEndDash();
}

void UCDashComponent::PlaySoundWave()
{
	CheckNull(Sound);
	CheckNull(OwnerCharacter);

	UWorld* world = OwnerCharacter->GetWorld();
	FVector location = OwnerCharacter->GetActorLocation();


	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void UCDashComponent::CheckEvadeHit()
{
	CheckNull(OwnerCharacter);
	CheckTrue(bIsEvadeSuccessed);

	FVector startLocation = PrevLocation;
	FVector endLocation = OwnerCharacter->GetActorLocation();
	float sphereRadius = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius() * 2.0f;

	// 충돌 저장할 배열 
	TArray<FHitResult> hitResults;

	// 트레이스 설정 
	FCollisionQueryParams tracePramams;
	tracePramams.AddIgnoredActor(OwnerCharacter);

	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	objectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	bool bHit = GetWorld()->SweepMultiByObjectType(
		hitResults,
		startLocation,
		endLocation,
		FQuat::Identity,
		objectQueryParams,
		FCollisionShape::MakeSphere(sphereRadius),
		tracePramams
	);

#ifdef  LOG_UCDashComponent

	// 트레이스 디버깅 (시각적으로 확인)
	DrawDebugSphere(GetWorld(), startLocation, sphereRadius, 12, FColor::Red, false, 1.0f);
	DrawDebugSphere(GetWorld(), endLocation, sphereRadius, 12, FColor::Blue, false, 1.0f);
	DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Green, false, 1.0f, 0, 2.0f);

#endif //  LOG_UCDashComponent

	if (bHit)
	{
		for (FHitResult& hit : hitResults)
		{
			AActor* HitActor = hit.GetActor();
			if (HitActor != nullptr /*&& HitActor->Implements<UAttackInterface>()*/) // 공격 관련 인터페이스 있는지 확인
			{
				IAttackInterface* ia = Cast<IAttackInterface>(HitActor);
				if (ia == nullptr)
					continue;
				if (ia->IsEnable() == false)
					continue;
				// 내가 주인이면 그건 패스
				if (ia->GetDamageSource() == OwnerCharacter)
					continue;

				// 회피 성공 처리
				HandleEvade();

				return;
			}
		}

		// 현재 위치를 다음 프레임의 PreviousLocation으로 설정
		//PrevLocation = endLocation;
	}
}

void UCDashComponent::Destroy_SingleGhostTrail()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]()
	{
		// 다 지워서 없다면 타이머 제거  
		if (GhostTrails.Num() <= 0)
		{
			//FLog::Log("Clear Traisl!!");
			OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);

			return;
		}

		for (int32 i = 0; i < GhostTrails.Num(); i++)
		{
			ACGhostTrail* ghost = GhostTrails[i];

			if (!!ghost)
			{
				ghost->Destroy();
				GhostTrails.RemoveAt(i);

				return;
			}
		}
	});


	// 타이머 설정 (예: 0.5초 후 제거)
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, TimerDelegate, 0.075f, true);
}

FVector UCDashComponent::GetGroundNormal()
{
	FHitResult hitResult;
	FVector start = OwnerCharacter->GetActorLocation();
	FVector end = start - FVector(0.0f, 0.0f, 1000.0f);
	
	FCollisionQueryParams params; 
	params.AddIgnoredActor(OwnerCharacter); 

	GetWorld()->LineTraceSingleByChannel(hitResult, start, end, ECC_Visibility, params);

	if (hitResult.IsValidBlockingHit())
	{
		return hitResult.Normal;
	}

	return FVector::ZAxisVector;
}


// Handle
//-----------------------------------------------------------------------------
void UCDashComponent::HandleBeginDash()
{
	CheckNull(State);
	CheckNull(Camera);
	CheckNull(Weapon);

	bIsDashing = true;

	// PostProcess
	if (!!Camera)
	{
		Camera->PostProcessSettings.bOverride_MotionBlurAmount = true;
		Camera->PostProcessSettings.MotionBlurAmount = BlurAmount;
	}

	//Sound
	PlaySoundWave();

	// State Change
	if (!!State)
	{
		if (State->IsEquipMode() == true)
		{
			if (!!Weapon->GetEquipment())
			{
				if (Weapon->GetEquipment()->GetBeginEquip() == false)
					Weapon->GetEquipment()->Begin_Equip();
			}
		}

		State->SetDashMode();
	}
	

	if (Weapon->GetEquipment() != nullptr && Weapon->GetEquipment()->GetBeginEquip() == true)
		Weapon->GetEquipment()->End_Equip();
}

void UCDashComponent::HandleEndDash()
{
	bIsDashing = false;

	CheckNull(State);
	CheckNull(Camera);
	CheckNull(Weapon);
	CheckNull(OwnerCharacter);


	// PostProcess 
	Camera->PostProcessSettings.bOverride_MotionBlurAmount = false;
	Camera->PostProcessSettings.MotionBlurAmount = 0.0f;


	if (State->IsEquipMode() == true)
	{
		if (Weapon->GetEquipment()->GetBeginEquip() == false)
			Weapon->GetEquipment()->Begin_Equip();
	}

	// 공격 중이였다면 공격 종료 
	if (Weapon->GetDoAction() != nullptr)
		Weapon->GetDoAction()->End_DoAction();

	if (State->IsIdleMode() == false)
	{
		State->SetIdleMode();
	}

	bIsEvadeSuccessed = false;
}

void UCDashComponent::HandleEvade()
{
	CheckNull(GhostTrailClass);

	// 이미 성공했다면 추가 처리 안함.
	CheckTrue(bIsEvadeSuccessed);

	//FLog::Log("Success Evade");
	bIsEvadeSuccessed = true;

	FTimerDelegate spawnDelegate;
	spawnDelegate.BindLambda([&]()
	{
		if (CurrentTrailCount >= TrailCount)
		{
			CurrentTrailCount = 0;
			Destroy_SingleGhostTrail();

			GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);

			return;
		}

		CurrentTrailCount++;

		FVector location = OwnerCharacter->GetActorLocation();

		FVector dashProgress = OwnerCharacter->GetActorLocation() - PrevLocation;
		float spawnRatio = (float)CurrentTrailCount / TrailCount;
		/*FVector spawnLocation = FMath::Lerp(PrevLocation, location, spawnLerp);*/
		FVector spawnLocation = PrevLocation + (dashProgress * spawnRatio);
		//FLog::Print("PrevLocation " + PrevLocation.ToString() + " CurrentLocation " + OwnerCharacter->GetActorLocation().ToString(), 4043);

		FActorSpawnParameters param;
		param.Owner = OwnerCharacter;
		param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		FTransform transform;
		//location.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		spawnLocation.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		transform.SetTranslation(spawnLocation);

		ACGhostTrail* GhostTrail = OwnerCharacter->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, param);
		GhostTrails.Add(GhostTrail);
	});

	/*float dashDistance = FVector::Dist(OwnerCharacter->GetActorLocation(), PrevLocation);
	float spawnInterval = dashDistance / TrailCount;*/

	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, spawnDelegate, 0.025f, true);
}

void UCDashComponent::CreateEvadeOverlap(const FVector& InPrevLocation)
{
	if (!!DashAvoidanceCollision)
	{
		if (DashAvoidanceCollision->IsRegistered())
		{
			DashAvoidanceCollision->DestroyComponent();
		}
		DashAvoidanceCollision = nullptr;
	}

	DashAvoidanceCollision = NewObject<USphereComponent>(this);

	// 크기 및 위치 설정
	DashAvoidanceCollision->SetSphereRadius(34.0f);
	DashAvoidanceCollision->SetWorldLocation(InPrevLocation); // 이전 위치에 생성

	// 충돌 설정
	DashAvoidanceCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DashAvoidanceCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // 커스텀 채널 설정
	DashAvoidanceCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DashAvoidanceCollision->SetCollisionProfileName("CharacterMesh"); // 특정 채널에만 반응

	// 디버그 확인용으로 보이게 설정
	DashAvoidanceCollision->SetHiddenInGame(false);
	DashAvoidanceCollision->SetVisibility(true);

	// 컴포넌트 등록
	DashAvoidanceCollision->RegisterComponent();

	// 이벤트 바인딩
	DashAvoidanceCollision->OnComponentBeginOverlap.AddDynamic(this, &UCDashComponent::OnComponentBeginOverlap);


	//// 일정 시간이 지나면 컴포넌트 제거
	//FTimerDelegate TimerDelegate;
	//TimerDelegate.BindLambda([=]()
	//{
	//	if (DashAvoidanceCollision)
	//	{
	//		DashAvoidanceCollision->DestroyComponent();
	//	}
	//});

	//// 타이머 설정 (예: 0.5초 후 제거)
	//GetWorld()->GetTimerManager().SetTimer(DashOverlapTimer, TimerDelegate, 0.5f, false);
}

void UCDashComponent::DestroyEvadeOverlap()
{
	if (!!DashAvoidanceCollision)
	{
		if (DashAvoidanceCollision->IsRegistered())
		{
			DashAvoidanceCollision->DestroyComponent();
		}
		DashAvoidanceCollision = nullptr;
	}
}

void UCDashComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 자신이 충돌되면 제외 
	CheckTrue(OtherActor == OwnerCharacter);

	FLog::Print(" Is Evade!" + OtherActor->GetName());
}

void UCDashComponent::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	// 대쉬 끝나고 대쉬 엔드 노티파이가 제대로 호출이 안되는 경우 (보통 끝나고 나서 피격) 
	// 그니까 대쉬 상태에서 다른 상태로 되면 그냥 HandleEnd 콜하면될듯?
	if (InPrevType == EStateType::Dash && InPrevType != InNewType)
		HandleEndDash();
}

