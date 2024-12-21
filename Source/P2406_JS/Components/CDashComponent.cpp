#include "Components/CDashComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Camera/CameraComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CWeaponComponent.h"
#include <Components/SphereComponent.h>
#include "Components/CapsuleComponent.h"

#include "Characters/CGhostTrail.h"


UCDashComponent::UCDashComponent()
{
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
}



void UCDashComponent::OnDash()
{
	CheckNull(Movement);

	DashAction();
}

void UCDashComponent::DashAction()
{
	CheckNull(Movement);
	CheckTrue(State->IsDashMode());
	//CheckNull(target);

	FVector* input = Movement->GetInputDirection();
	if (input == nullptr)
		return;
	inputVec = *input;

	// 자유 카메라 모드 일 때 
	if (Weapon->GetEquipment() == nullptr
		|| Weapon->GetEquipment()->GetControlRotation() == false)
	{
		// 전방
		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Forward]);
	}
	// 고정 회전 모드 일때
	else
	{
		// 전방
		DashDirection dir = DashDirection::Forward;
		// 후방 
		if (input->X <= 0)
			dir = DashDirection::Back;

		if (input->Y > 0)
			dir = DashDirection::Right;
		else if (input->Y < 0)
			dir = DashDirection::Left;

		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)dir]);
	}
}

void UCDashComponent::Begin_DashSpeed()
{
	FVector dashDir = OwnerCharacter->GetActorForwardVector();

	if (Weapon->GetEquipment() != nullptr
		&& Weapon->GetEquipment()->GetControlRotation() == true)
	{
		double xAxis = inputVec.X;
		if (inputVec.X == 0 && inputVec.Y == 0)
			xAxis = -1;

		dashDir = xAxis * OwnerCharacter->GetActorForwardVector() +
			inputVec.Y * OwnerCharacter->GetActorRightVector();
	}


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
	//CreateEvadeOverlap(OwnerCharacter->GetActorLocation());

	OwnerCharacter->LaunchCharacter(dashDir * DashSpeed, true, true);


	CheckNull(Weapon);
	CheckNull(Weapon->GetEquipment());

	if (Weapon->GetEquipment()->GetBeginEquip() == true)
		Weapon->GetEquipment()->End_Equip();
}

void UCDashComponent::End_DashSpeed()
{
	//Movement->SetSpeed(ESpeedType::Run);
	OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);

	CheckNull(Weapon);

	if (!!Camera)
	{
		Camera->PostProcessSettings.bOverride_MotionBlurAmount = false;
		Camera->PostProcessSettings.MotionBlurAmount = 0.0f;
	}

	CheckNull(State);
	if (State->IsEquipMode() == true)
	{
		if (Weapon->GetEquipment()->GetBeginEquip() == false)
			Weapon->GetEquipment()->Begin_Equip();
	}

	// 공격 중이였다면 공격 종료 
	if(Weapon->GetDoAction() != nullptr)
		Weapon->GetDoAction()->End_DoAction();

	if (State->IsIdleMode() == false)
	{
		State->SetIdleMode();
	}
}

void UCDashComponent::PlaySoundWave()
{
	CheckNull(Sound);
	CheckNull(OwnerCharacter);

	UWorld* world = OwnerCharacter->GetWorld();
	FVector location = OwnerCharacter->GetActorLocation();


	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
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
	//OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(DashOverlapTimer, TimerDelegate, 0.5f, false);
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
	FLog::Print(" Is Evade!");

	if (!!GhostTrailClass)
	{
		// 고스트 트레일 스폰 
		FTimerDelegate SpawnDelegate;
		SpawnDelegate.BindLambda([this]()
		{
			FVector location = OwnerCharacter->GetActorLocation();
			location.Z -= OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

			FActorSpawnParameters param;
			param.Owner = OwnerCharacter;
			param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


			FTransform transform;
			transform.SetTranslation(location);


			ACGhostTrail* GhostTrail = OwnerCharacter->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, transform, param);

			GhostTrails.Add(GhostTrail);


			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([this]()
			{
				for (ACGhostTrail* ghost : GhostTrails)
				{
					if (ghost)
						ghost->Destroy();
				}
			});

			// 타이머 설정 (예: 0.5초 후 제거)
			OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(GhostTimer, TimerDelegate, 0.5f, false);
		});

		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(SpawnTimer, SpawnDelegate, SpwanInterval, true, 0.f);
	}
}

