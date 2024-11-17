#include "Components/CDashComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/CEquipment.h"
#include "Camera/CameraComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CWeaponComponent.h"
#include "Characters/CGhostTrail.h"
#include <Components/BoxComponent.h>

UCDashComponent::UCDashComponent()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

}


void UCDashComponent::BeginPlay()
{
	Super::BeginPlay();

	if (!!OwnerCharacter)
	{
		Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
		target = CHelpers::GetComponent<UCTargetComponent>(OwnerCharacter);
		Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
		State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
		Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
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
		DashDirection dir = DashDirection::Forward;
		// 전방
		if (input->X > 0)
		{
			if (input->Y > 0)
				dir = DashDirection::Right;
			else if(input->Y < 0)
				dir = DashDirection::Left;
		}
		// 후방 
		else
		{
			dir = DashDirection::Back;
			if (input->Y > 0)
				dir = DashDirection::Right;
			else if (input->Y < 0)
				dir = DashDirection::Left;
		}

		OwnerCharacter->PlayAnimMontage(DashMontages[(int32)dir]);
	}
	//// 후방 
	//else if(input->X < 0)
	//{
	//	OwnerCharacter->PlayAnimMontage(DashMontages[(int32)DashDirection::Back]);
	//}
}

void UCDashComponent::PlayEvadeEffect()
{
	FVector effectLocation = OwnerCharacter->GetActorLocation();
	FRotator effectRotation = OwnerCharacter->GetActorRotation();
	CLog::Print("Evade !! ");
	if (!!GhostTrailClass)
	{
		GhostTrail = OwnerCharacter->GetWorld()->SpawnActor<ACGhostTrail>(GhostTrailClass, effectLocation, effectRotation);

		FTimerDelegate timerDelegate;
		timerDelegate.BindLambda([this]()
		{
			if(!!GhostTrail)
				GhostTrail->Destroy();
		});

		OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(GhostTimer, timerDelegate, 0.5f, false);
	}

}

void UCDashComponent::Begin_DashSpeed()
{
	FVector dashDir = OwnerCharacter->GetActorForwardVector();
	
	if (Weapon->GetEquipment() != nullptr
		&& Weapon->GetEquipment()->GetControlRotation() == true)
	{
		double xAxis = inputVec.X == 0 ? -1 : inputVec.X;
		dashDir = xAxis * OwnerCharacter->GetActorForwardVector() +
			inputVec.Y * OwnerCharacter->GetActorRightVector();
	}

	OwnerCharacter->LaunchCharacter(dashDir * DashSpeed, true, true);

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
			if (Weapon->GetEquipment()->GetBeginEquip() == false)
				Weapon->GetEquipment()->Begin_Equip();
		}

		State->SetDashMode();
	}
	CreateEvadeOverlap(OwnerCharacter->GetActorLocation());


	CheckNull(Weapon);
	CheckNull(Weapon->GetEquipment());

	if (Weapon->GetEquipment()->GetBeginEquip() == true)
		Weapon->GetEquipment()->End_Equip();
}

void UCDashComponent::End_DashSpeed()
{
	//Movement->SetSpeed(ESpeedType::Run);
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
	// 예: 이전 위치에 Box 콜리전 생성
	UBoxComponent* dashAvoidanceCollsion = NewObject<UBoxComponent>(this);
	dashAvoidanceCollsion->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	dashAvoidanceCollsion->SetWorldLocation(InPrevLocation);
	dashAvoidanceCollsion->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	dashAvoidanceCollsion->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1); // 커스텀 채널 설정
	dashAvoidanceCollsion->SetCollisionResponseToAllChannels(ECR_Ignore);
	dashAvoidanceCollsion->SetCollisionResponseToChannel(ECC_GameTraceChannel2, ECR_Overlap); // 적 공격에만 반응
	dashAvoidanceCollsion->AttachToComponent(OwnerCharacter->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);

	dashAvoidanceCollsion->RegisterComponent();

	dashAvoidanceCollsion->SetHiddenInGame(false);
	dashAvoidanceCollsion->SetVisibility(true);


	dashAvoidanceCollsion->OnComponentBeginOverlap.AddDynamic(this, &UCDashComponent::OnComponentBeginOverlap);

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
	{
		if (!!dashAvoidanceCollsion)
			dashAvoidanceCollsion->DestroyComponent();
	});
	//TODO: 대쉬를 연속하면 이전 타이머가 죽어서 바로 콜리전이 안사라짐.
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(DashOverlapTimer, timerDelegate, 0.5f, false);
}

void UCDashComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CLog::Print(" Is Evade!");
}

