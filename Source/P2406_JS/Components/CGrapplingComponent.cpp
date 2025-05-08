﻿#include "Components/CGrapplingComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CableComponent.h"

#include "Components/CZoomComponent.h"

UCGrapplingComponent::UCGrapplingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCGrapplingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter != nullptr)
	{
		Cable = FHelpers::GetComponent<UCableComponent>(OwnerCharacter);
		Zoom = FHelpers::GetComponent<UCZoomComponent>(OwnerCharacter);
	}
}

void UCGrapplingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Grapple_1(DeltaTime);
	//Grapple_2(DeltaTime);

	if (Cable != nullptr)
	{
		FVector start = OwnerCharacter->GetActorLocation();

		Cable->SetWorldLocation(start);

		// 끝점은 고정된 타겟 위치
		FVector desireLoc = TargetLocation + FVector(0, 0, +34);
		FVector LocalEndLocation = Cable->GetComponentTransform().InverseTransformPosition(desireLoc);
		Cable->EndLocation = LocalEndLocation;
	}

	TryGrapplingTarget();
}

void UCGrapplingComponent::OnGrapple()
{
	CheckNull(OwnerCharacter);
	CheckTrue(bIsGrappling);


	bGrappleEnd = false;

	SetTarget();

	OwnerCharacter->GetCharacterMovement()->Velocity.Zero();
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	OwnerCharacter->PlayAnimMontage(GrapplingMontage, PlayRate);
}

void UCGrapplingComponent::PullTowardsTarget()
{
	FVector Direction = (TargetLocation - OwnerCharacter->GetActorLocation()).GetSafeNormal();

	OwnerCharacter->LaunchCharacter(Direction * PullSpeed, true, true);
}

void UCGrapplingComponent::TryGrapplingTarget()
{
	CheckNull(Zoom);
	
	if (Zoom->GetGrapplingZoomMode() == true)
	{
		DYNAMIC_EVENT_CALL_ONE_PARAM(OnGrapplingZoomMode, true);

		return; 
	}
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnGrapplingZoomMode, false);
}

void UCGrapplingComponent::OnGrappling_Pressed()
{
	CheckNull(OwnerCharacter);
	CheckTrue(bIsGrappling);

	bIsGrappling = true;
	OnGrapple();
}

void UCGrapplingComponent::OnGrappling_Released()
{
	CheckNull(OwnerCharacter);
	//CheckTrue(bIsGrappling);

	bIsGrappling = false;
}

void UCGrapplingComponent::ResetGrapple()
{
	bIsGrappling = false;
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void UCGrapplingComponent::InterruptGrapple()
{
	End_DoGrappling();
}

void UCGrapplingComponent::SetTarget()
{

	FVector CameraLocation;
	FRotator CamraRotation;

	OwnerCharacter->GetController()->GetPlayerViewPoint(CameraLocation,
		CamraRotation);

	FHitResult HitResult;
	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = Start + (CamraRotation.Vector() * MaxLineDistance);  // 원하는 거리로 트레이스
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		TargetLocation = HitResult.Location;
		// 여기서 타겟 위치를 설정
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.0f);
	}
	else
	{
		//FLog::Print("No Colloisions!");
		TargetLocation = End;
		DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 5.0f);
		//PullTowardsTarget();
	}
}

void UCGrapplingComponent::InstallrapplingRope()
{
	CheckNull(OwnerCharacter);
	CheckNull(Cable);


	Cable->AttachEndTo = FComponentReference();

	Cable->bAttachEnd = false; // 끝점 연결 비활성화
	//Cable->EndLocation = TargetLocation;// 그래플링 훅 설치 

	FVector LocalEndLocation = Cable->GetComponentTransform().InverseTransformPosition(TargetLocation);
	Cable->EndLocation = LocalEndLocation;
	Cable->SetHiddenInGame(false);
}

void UCGrapplingComponent::UninstallrapplingRope()
{
	CheckNull(Cable);


	Cable->SetHiddenInGame(true);
}

void UCGrapplingComponent::Begin_DoGrappling()
{
	CheckNull(OwnerCharacter);
	CheckNull(OwnerCharacter->GetCharacterMovement());


	// 해당 방향으로 캐릭터 회전하기 
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	OwnerCharacter->SetActorRotation(TargetRotator);
	OwnerCharacter->StopAnimMontage(GrapplingMontage);

	bIsGrappling = true;

	InstallrapplingRope(); 
}

void UCGrapplingComponent::End_DoGrappling()
{
	bGrappleEnd = true;
	ResetGrapple();
	if (!!GrapplingMontage)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Grappling_End"), GrapplingMontage);
	}

	UninstallrapplingRope();
}


void UCGrapplingComponent::Grapple_1(float InDetaTime)
{
	CheckFalse(bIsGrappling);

	// 이기능은 공중 대시에 가깝다 
	if (bGrappleEnd == false)
	{
		// 목표 지점까지 남은 거리
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal(); // 방향 벡터
		FVector NewLocation = CurrentLocation + (Direction * PullSpeed * InDetaTime); // 속도 * 시간 = 거리

		OwnerCharacter->SetActorLocation(NewLocation);

		// 목표 지점에 도달했는지 확인
		if (FVector::Dist(NewLocation, TargetLocation) < DistanceThreshold)
		{
			End_DoGrappling();
		}
	}
}

void UCGrapplingComponent::Grapple_2(float InDetaTime)
{
	CheckFalse(bIsGrappling);

	//TODO: 천천히 가다가 도중에 중단하거나 버튼 떼면 거기서 멈추게하면될듯 
	if (bGrappleEnd == false)
	{
		FVector NewLocation = FMath::Lerp(OwnerCharacter->GetActorLocation(), TargetLocation, PullSpeed);
		OwnerCharacter->SetActorLocation(NewLocation);

		// 목표 지점에 도달했는지 확인
		if (FVector::Dist(OwnerCharacter->GetActorLocation(), TargetLocation) < DistanceThreshold)
		{
			// 그래플링 완료 처리 로직
			//bIsGrappling = false;
			bGrappleEnd = true;
		}
	}
}
