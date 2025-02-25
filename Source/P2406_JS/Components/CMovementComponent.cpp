#include "Components/CMovementComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

//#define LOG_UCMovementComponent

UCMovementComponent::UCMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

}

void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#ifdef LOG_UCMovementComponent
	FVector start = OwnerCharacter->GetActorLocation();
	FVector velocity = OwnerCharacter->GetVelocity();

	CheckTrue(velocity.IsNearlyZero());

	FVector end = start + velocity.GetSafeNormal() * 200.0f;

	// 화살표 디버그 그리기
	DrawDebugDirectionalArrow(
		GetWorld(),
		start,
		end,
		100.0f, // 화살표 크기
		FColor::Green, // 화살표 색상
		false,
		0.0f, // 지속 시간
		0,
		5.0f // 두께
	);

	FLog::Print("X : " + FString::SanitizeFloat(InputDirection.X), 3924);
	FLog::Print("Y : " + FString::SanitizeFloat(InputDirection.Y), 3925);
	FLog::Print("Speed X : " + FString::SanitizeFloat(velocity.X), 3926);
	FLog::Print("Speed Y : " + FString::SanitizeFloat(velocity.Y), 3927);

	FRotator rotator = FRotator(0, OwnerCharacter->GetActorRotation().Yaw, 0);
	FVector dir_Start = start;
	FVector dir_End = dir_Start + FQuat(rotator).GetForwardVector()* 200.0f;

	DrawDebugDirectionalArrow(
		GetWorld(),
		dir_Start,
		dir_End,
		100.0f, // 화살표 크기
		FColor::Red, // 화살표 색상
		false,
		-1.0f, // 지속 시간
		0,
		5.0f // 두께
	);
#endif
}

void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnRun()
{
	SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(int32)InType];
}

void UCMovementComponent::OnMoveForward(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);

	InputDirection.X = direction.X * InAxis;
}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	CheckFalse(bCanMove);

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);

	InputDirection.Y = direction.Y * InAxis;
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	CheckTrue(bFixedCamera);

	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
	bCanControlRotaion = true; 
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	bCanControlRotaion = false; 
}

void UCMovementComponent::Move()
{
	bCanMove = true;
}

void UCMovementComponent::Stop()
{
	bCanMove = false;
}

