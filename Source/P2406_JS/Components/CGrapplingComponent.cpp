#include "Components/CGrapplingComponent.h"
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
		case EGrapplingState::Aim:
			SearchGrapplingAbleTarget();
			break;
		case EGrapplingState::Begin:
			break;
		case EGrapplingState::Action:
			PerformGrapple(DeltaTime);

			if (Cable != nullptr)
			{
				FVector start = OwnerCharacter->GetActorLocation();

				Cable->SetWorldLocation(start);

				// 끝점은 고정된 타겟 위치
				FVector desireLoc = TargetLocation + FVector(0, 0, +34);
				FVector LocalEndLocation = Cable->GetComponentTransform().InverseTransformPosition(desireLoc);
				Cable->EndLocation = LocalEndLocation;
			}

			break;
		case EGrapplingState::End:
			break;
	}
}

void UCGrapplingComponent::DoAction()
{
	FLog::Log("Grapple - DoAction");
	switch (GrapMode)
	{
		case EGrapplingMode::Grappling:
			DoAction_Grappling();
			break; 
		case EGrapplingMode::Hook:
			DoAction_Hook(); 
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
	GrapplingState = EGrapplingState::Aim;
}

void UCGrapplingComponent::Released()
{
	CheckNull(OwnerCharacter);
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	CheckNull(player);

	DYNAMIC_EVENT_CALL(OnGraplingReleased);
	player->OnInputStateChanged(EInputState::Grapple);
	GrapplingState = EGrapplingState::Max;
}


void UCGrapplingComponent::PullTargetToSelf(AActor* InTarget)
{
	CheckNull(OwnerCharacter);
	CheckNull(InTarget);

	ACharacter* targeCharacter = Cast<ACharacter>(InTarget);
	CheckNull(targeCharacter);

	FLog::Log("Grapple - PullTargetToSelf");
	FVector mylocation = OwnerCharacter->GetActorLocation();
	FVector targetLocation = targeCharacter->GetActorLocation();

	FVector dir = (mylocation - targetLocation).GetSafeNormal2D();

	float myRadius = 0;
	float myHalfHeight = 0;

	OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleSize(myRadius, myHalfHeight);

	float targetRadius = 0.0f;
	float targetHalfHeight = 0;

	targeCharacter->GetCapsuleComponent()->GetScaledCapsuleSize(targetRadius, targetHalfHeight);

	//TODO: 캐릭터가 아닌 오브젝트일때 
	//FVector Origin, BoxExtent;
	//Target->GetActorBounds(true, Origin, BoxExtent);

	//float TargetRadius = FMath::Max(BoxExtent.X, BoxExtent.Y);  // 원형 대체


	float safeDistance = myRadius + targetRadius + 10.f;

	//OwnerCharacter->GetActorLocation().ForwardVector* safeDistance;
	TargetLocation = targetLocation;

	// 동작 수행
	OwnerCharacter->PlayAnimMontage(HookMontage, PlayRate);
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

	GrapMode = EGrapplingMode::Hook;
}

void UCGrapplingComponent::ResetGrapple()
{
	GrapplingState = EGrapplingState::Max;
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

	FLog::Log("Grapple - InstallGrapplingRope");
	Cable->AttachEndTo = FComponentReference();

	Cable->bAttachEnd = false; // 끝점 연결 비활성화
	//Cable->EndLocation = TargetLocation;// 그래플링 훅 설치 

	FVector LocalEndLocation = Cable->GetComponentTransform().InverseTransformPosition(TargetLocation);
	Cable->EndLocation = LocalEndLocation;
	Cable->SetHiddenInGame(false);

	GrapplingState = EGrapplingState::Action;
}

void UCGrapplingComponent::UninstallrapplingRope()
{
	CheckNull(Cable);
	FLog::Log("Grapple - UninstallrapplingRope");

	Cable->SetHiddenInGame(true);
}



void UCGrapplingComponent::DoAction_Grappling()
{
	CheckNull(OwnerCharacter);

	SetTarget();

	OwnerCharacter->GetCharacterMovement()->Velocity.Zero();
	OwnerCharacter->GetCharacterMovement()->StopMovementImmediately();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	OwnerCharacter->PlayAnimMontage(GrapplingMontage, PlayRate);
}

void UCGrapplingComponent::DoAction_Hook()
{
	FLog::Log("Grapple - DoAction_Hook");
	// TODO:: 로프 발사
	// 1. 타겟팅된 대상이 있는가?
	CheckNull(GrappleTarget); 

	// 2. 대상이 적인가?
	ACBaseCharacter* bc = Cast<ACBaseCharacter>(GrappleTarget);
	
	// 2-1. 적이라면 어떤 상태인가?
	if (bc != nullptr)
	{
		UCConditionComponent* condition = FHelpers::GetComponent<UCConditionComponent>(bc);
		CheckNull(condition);
		// 다운 상태라면 걔를 내앞으로 끌고 온다.
		if (condition->GetDownCondition())
		{
			// 다운 상태 해제 시킴 
			condition->RemoveDownCondition();
			PullTargetToSelf(bc);
		}
		// 다운 상태가 아니라면 내가 날아감.
		else
		{

		}
	}

}

void UCGrapplingComponent::Begin_Grappling()
{
	GrapplingState = EGrapplingState::Begin;

	switch (GrapMode)
	{
		case EGrapplingMode::Grappling:
			Begin_DoGrappling(); 
			break;
		case EGrapplingMode::Hook:
			Begin_Hooking(); 
			break;
	}
}

void UCGrapplingComponent::End_Grappling()
{
	switch (GrapMode)
	{
		case EGrapplingMode::Grappling:
			End_DoGrappling();
			break;
		case EGrapplingMode::Hook:
			End_Hooking();
			break;
	}
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

	InstallGrapplingRope();
}

void UCGrapplingComponent::End_DoGrappling()
{
	GrapplingState = EGrapplingState::End;
	
	ResetGrapple();
	if (!!GrapplingMontage)
	{
		OwnerCharacter->GetMesh()->GetAnimInstance()->Montage_JumpToSection(FName("Grappling_End"), GrapplingMontage);
	}

	UninstallrapplingRope();
}

void UCGrapplingComponent::Begin_Hooking()
{
	InstallGrapplingRope();
}

void UCGrapplingComponent::End_Hooking()
{
	// 끌어 오기 시작 
	//ResetGrapple();
	//UninstallrapplingRope();
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

void UCGrapplingComponent::Grapple(AActor* InTarget, float InDetaTime)
{
	CheckNull(InTarget);

	// 목표 지점까지 남은 거리
	FVector currentLocation = OwnerCharacter->GetActorLocation();
	FVector direction = (TargetLocation - currentLocation).GetSafeNormal(); // 방향 벡터
	FVector newLocation = currentLocation + (direction * PullSpeed * InDetaTime); // 속도 * 시간 = 거리

	InTarget->SetActorLocation(newLocation);

	// 목표 지점에 도달했는지 확인
	if (FVector::Dist(newLocation, TargetLocation) < DistanceThreshold)
	{
		FLog::Log("Grapple - Grapple Arrive");
		End_DoGrappling();
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
