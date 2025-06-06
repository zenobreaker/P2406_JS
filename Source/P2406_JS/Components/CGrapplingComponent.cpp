﻿#include "Components/CGrapplingComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CableComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"

#include "Characters/CPlayer.h"
#include "Components/CZoomComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CConditionComponent.h"


UCGrapplingComponent::UCGrapplingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	FHelpers::GetClass<UUserWidget>(&TargetUiClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/GrapplingTargetCrossHair.GrapplingTargetCrossHair_C'");
}


void UCGrapplingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (OwnerCharacter != nullptr)
	{
		Cable = FHelpers::GetComponent<UCableComponent>(OwnerCharacter);
		Target = FHelpers::GetComponent<UCTargetComponent>(OwnerCharacter);
		Zoom = FHelpers::GetComponent<UCZoomComponent>(OwnerCharacter);
	}

	GrapplingState = EGrapplingState::Max;
	GrapMode = EGrapplingMode::Max;
}

void UCGrapplingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	TryGrapplingTarget();

	switch (GrapplingState)
	{
		case EGrapplingState::Idle:
			break;
		case EGrapplingState::Aiming:
			SearchGrapplingAbleTarget();
			break;
		case EGrapplingState::TargetLocked:
			break;
		case EGrapplingState::Firing:
			break;
		case EGrapplingState::Pulling:
			PerformGrapple(DeltaTime);
			break;
		case EGrapplingState::Finish:
			break;
	}
}

void UCGrapplingComponent::DoAction()
{
	FLog::Log("Grapple - DoAction");
	GrapplingState = EGrapplingState::Firing;

	switch (GrapMode)
	{
		case EGrapplingMode::Grappling:
			SetTarget();
			OwnerCharacter->GetCharacterMovement()->Velocity.Zero();
			OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
			OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			OwnerCharacter->PlayAnimMontage(GrapplingMontage, PlayRate);
			break;
		case EGrapplingMode::Hook:
			// 동작 수행
			OwnerCharacter->PlayAnimMontage(HookMontage, PlayRate);
			break;
	}
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
		bGrapplingZoomMode = true;

		return;
	}

	DYNAMIC_EVENT_CALL_ONE_PARAM(OnGrapplingZoomMode, false);
	bGrapplingZoomMode = false;

	if (TargetUi != nullptr)
		TargetUi->SetVisibility(false);
}

void UCGrapplingComponent::Pressed()
{
	CheckNull(OwnerCharacter);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	DYNAMIC_EVENT_CALL(OnGraplingPressed);
	player->OnInputStateChanged(EInputState::Grapple);

	if (bIsGrappling == false)
		GrapplingState = EGrapplingState::Aiming;
}

void UCGrapplingComponent::Released()
{
	CheckNull(OwnerCharacter);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);
	player->OnInputStateChanged(EInputState::Combat);
	
	
	ResetGrapple();
	bIsGrappling = false;
}

void UCGrapplingComponent::SearchGrapplingAbleTarget()
{
	CheckFalse(bGrapplingZoomMode);
	CheckNull(Target);

	AActor* target = Target->GetTarget();
	// 이미 타겟한게 있다면 여기에 동일하게 조준
	if (target != nullptr)
	{
		DrawGrapplingTargetUI(target);
		SetModeWithTargeType(target);
		return;
	}

	FVector location = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignores;
	ignores.Add(OwnerCharacter);

	// 없다면 계산해서 가져온다. 
	TArray<FHitResult> hitResults;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), location,
		location, TraceDistance,
		"Targeting", false, ignores,
		DrawDebug, hitResults, true);

	AActor* candidate = Target->GetNearlyFrontAngleActor(hitResults);
	CheckNull(candidate);
	FLog::Print("Target On", 1);
	DrawGrapplingTargetUI(candidate);
	SetModeWithTargeType(candidate);
}

void UCGrapplingComponent::SetModeWithTargeType(AActor* InTarget)
{
	CheckNull(InTarget);

	// 끌어올수있는 대상일까? 
	ACharacter* character = Cast<ACharacter>(InTarget);
	if (character != nullptr)
	{
		GrapMode = EGrapplingMode::Hook;
		return;
	}

	GrapMode = EGrapplingMode::Grappling;
}

void UCGrapplingComponent::DrawGrapplingTargetUI(AActor* InTarget)
{
	CheckNull(InTarget);
	CheckNull(TargetUiClass);

	FLog::Print("Target Draw", 2);

	// 사전에 UI가 있었다면 그 대상을 제거 
	if (GrappleTarget == nullptr || GrappleTarget != InTarget)
		GrappleTarget = InTarget;
	else if (GrappleTarget == InTarget)
		return;

	if (TargetUi != nullptr)
		TargetUi->DestroyComponent();

	FLog::Print("Target Draw UI ", 2);
	TargetUi = NewObject<UWidgetComponent>(this);
	TargetUi->AttachToComponent(InTarget->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	TargetUi->RegisterComponent();
	FTransform transform;
	transform.SetLocation(FVector(0, 0, 50));
	TargetUi->SetRelativeTransform(transform);
	TargetUi->SetWidgetClass(TargetUiClass);
	TargetUi->SetWidgetSpace(EWidgetSpace::Screen);
	TargetUi->SetDrawSize(FVector2D(100, 100)); // 적절한 크기 설정
	TargetUi->SetVisibility(true);
}

void UCGrapplingComponent::ResetGrapple()
{
	CheckNull(OwnerCharacter);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	if (bIsGrappling || GrapplingState != EGrapplingState::Max)
	{
		DYNAMIC_EVENT_CALL(OnGraplingReleased); 
		GrapplingState = EGrapplingState::Finish;
	}

	bGrappleTargetSetComplete = false; 
	GrappleTarget = nullptr;
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

void UCGrapplingComponent::InstallGrapplingRope()
{
	CheckNull(OwnerCharacter);
	CheckNull(Cable);
	CheckNull(GrappleTarget)
	
	FLog::Log("Grapple - InstallGrapplingRope");
	Cable->AttachEndTo = FComponentReference();

	Cable->bAttachEnd = true; 
	Cable->AttachEndTo.OtherActor = GrappleTarget;
	Cable->AttachEndTo.ComponentProperty = NAME_None;
	Cable->SetHiddenInGame(false);

	ProcessGrapple(GrappleTarget);
}

void UCGrapplingComponent::UninstallrapplingRope()
{
	CheckNull(Cable);
	FLog::Log("Grapple - UninstallrapplingRope");

	Cable->bAttachEnd = false; // 끝점 연결 비활성화
	Cable->AttachEndTo = FComponentReference(); // 비우기

	Cable->SetHiddenInGame(true);
}

void UCGrapplingComponent::Begin_DoGrappling()
{
	CheckNull(OwnerCharacter);
	CheckNull(OwnerCharacter->GetCharacterMovement());
	CheckNull(GrappleTarget)

	// 해당 방향으로 캐릭터 회전하기 
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, GrappleTarget->GetActorLocation());

	OwnerCharacter->SetActorRotation(TargetRotator);
	/*if(GrapMode == EGrapplingMode::Grappling)
		OwnerCharacter->StopAnimMontage(GrapplingMontage);*/

	// 로프 설치
	InstallGrapplingRope();
}

void UCGrapplingComponent::End_DoGrappling()
{
	CheckNull(GrappleTarget);
	CheckFalse(bGrappleTargetSetComplete);

	GrapplingState = EGrapplingState::Pulling;
}

void UCGrapplingComponent::Check_GrapleMode()
{
	switch (GrapMode)
	{
		case EGrapplingMode::Grappling:
			Check_Grapping();
			break;
		case EGrapplingMode::Hook:
			Check_Hooking();
			break;
	}
}

void UCGrapplingComponent::Check_Grapping()
{

}

void UCGrapplingComponent::Check_Hooking()
{

}

void UCGrapplingComponent::ProcessGrapple(AActor* InTarget)
{
	CheckNull(InTarget);
	//TODO: 캐릭터가 아닌 오브젝트일때 
	//FVector Origin, BoxExtent;
	//Target->GetActorBounds(true, Origin, BoxExtent);
	//float TargetRadius = FMath::Max(BoxExtent.X, BoxExtent.Y);  // 원형 대체


	// 대상이 적인가?
	ACBaseCharacter* bc = Cast<ACBaseCharacter>(InTarget);

	// 적이라면 어떤 상태인가?
	if (bc != nullptr)
	{
		UCConditionComponent* condition = FHelpers::GetComponent<UCConditionComponent>(bc);
		CheckNull(condition);
		// 다운 상태라면 걔를 내앞으로 끌고 온다.
		if (condition->GetDownCondition())
		{
			// 다운 상태 해제 시킴 
			condition->RemoveDownCondition();
		}
		// 다운 상태가 아니라면 내가 날아감.
		else
		{

		}
	}

	bGrappleTargetSetComplete = true; 
}

void UCGrapplingComponent::PerformGrapple(float InDeltaTime)
{
	switch (GrapMode)
	{
		case EGrapplingMode::Grappling:
			Grapple(OwnerCharacter, InDeltaTime);
			break;
		case EGrapplingMode::Hook:
			Grapple(GrappleTarget, InDeltaTime);
			break;
	}
}

void UCGrapplingComponent::Grapple(AActor* InTarget, float InDeltaTime)
{
	CheckNull(InTarget);

	// 목표 지점까지 남은 거리
	const FVector ownerLocation = OwnerCharacter->GetActorLocation();
	const FVector targetLocation = InTarget->GetActorLocation();
	FVector direction = FVector::ZeroVector;
	FVector goalLocation = FVector::ZeroVector;

	// 캡슐 사이 안전 거리 계산 
	float myRadius, myHalfHegiht;
	OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleSize(myRadius, myHalfHegiht);

	float targetRadius, targetHalfHeight;
	if (ACharacter* target = Cast<ACharacter>(InTarget))
		target->GetCapsuleComponent()->GetScaledCapsuleSize(targetRadius, targetHalfHeight);
	else
		targetRadius = 50.0f;

	// 충돌 방지 거리 
	float safeDistance = myRadius + targetRadius + 30.0f;

	// 목표 위치 설정 
	if (GrapMode == EGrapplingMode::Grappling)
	{
		// 내가 가는 경우
		direction = (targetLocation - ownerLocation).GetSafeNormal(); // 방향 벡터
		goalLocation == targetLocation - direction * safeDistance;

		FVector newLocation = FMath::VInterpTo(ownerLocation, goalLocation, InDeltaTime, InterpSpeed);
		OwnerCharacter->SetActorLocation(newLocation);

		// 목표 지점에 도달했는지 확인
		if (FVector::Dist(newLocation, goalLocation) <= DistanceThreshold)
		{
			FLog::Log("Grapple - Grapple Arrive");
			ResetGrapple();
			UninstallrapplingRope();
			if (!!GrapplingMontage)
			{
				OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Grappling_End"), GrapplingMontage);
			}
		}
	}
	// 적이 나한테 오는 경우 
	else
	{
		direction = (ownerLocation - targetLocation).GetSafeNormal(); // 방향 벡터
		goalLocation = ownerLocation - direction * safeDistance;
		FVector newLocation = FMath::VInterpTo(targetLocation, goalLocation, InDeltaTime, InterpSpeed);
		InTarget->SetActorLocation(newLocation);

		if (FVector::Dist(newLocation, goalLocation) <= DistanceThreshold)
		{
			FLog::Log("Grapple - Grapple Arrive");
			
			ResetGrapple();
			UninstallrapplingRope();
		}
	}


	// 디버그 시각화
	if (DrawDebug != EDrawDebugTrace::None)
	{
		DrawDebugSphere(GetWorld(), goalLocation, 20.f, 12, FColor::Green, false, 0.1f);
	}

}

void UCGrapplingComponent::Grapple_2(float InDetaTime)
{
	//TODO: 천천히 가다가 도중에 중단하거나 버튼 떼면 거기서 멈추게하면될듯 

	FVector NewLocation = FMath::Lerp(OwnerCharacter->GetActorLocation(), TargetLocation, PullSpeed);
	OwnerCharacter->SetActorLocation(NewLocation);

	// 목표 지점에 도달했는지 확인
	if (FVector::Dist(OwnerCharacter->GetActorLocation(), TargetLocation) < DistanceThreshold)
	{
		// 그래플링 완료 처리 로직
		//bIsGrappling = false;

	}
}
