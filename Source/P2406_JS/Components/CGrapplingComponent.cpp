#include "Components/CGrapplingComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCGrapplingComponent::UCGrapplingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCGrapplingComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCGrapplingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Grapple_1(DeltaTime);
	//Grapple_2(DeltaTime);
}

void UCGrapplingComponent::OnGrapple()
{
	CheckNull(OwnerCharacter);
	CheckTrue(bIsGrappling);

	bIsGrappling = true;
	bGrappleEnd = false;

	FVector CameraLocation;
	FRotator CamraRotation;

	OwnerCharacter->GetController()->GetPlayerViewPoint(CameraLocation,
		CamraRotation);

	FHitResult HitResult;
	FVector Start = OwnerCharacter->GetActorLocation();
	FVector End = Start + (CamraRotation.Vector() * MaxLineDistance);  // ���ϴ� �Ÿ��� Ʈ���̽�
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams))
	{
		TargetLocation = HitResult.Location;
		// ���⼭ Ÿ�� ��ġ�� ����
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.0f);
	}
	else
	{
		CLog::Print("No Colloisions!");
		TargetLocation = End;
		DrawDebugLine(GetWorld(), Start, End, FColor::Yellow, false, 5.0f);
		//PullTowardsTarget();
	}

}

void UCGrapplingComponent::PullTowardsTarget()
{
	FVector Direction = (TargetLocation - OwnerCharacter->GetActorLocation()).GetSafeNormal();

	OwnerCharacter->LaunchCharacter(Direction * PullSpeed, true, true);
}

void UCGrapplingComponent::OnGrappling_Pressed()
{
	CheckNull(OwnerCharacter);
	CheckTrue(bIsGrappling);

	OnGrapple();
	//bIsGrappling = true;
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
	ResetGrapple();
}

void UCGrapplingComponent::Begin_DoGrappling()
{
	CheckNull(OwnerCharacter);
	CheckNull(OwnerCharacter->GetCharacterMovement());

	OwnerCharacter->GetCharacterMovement()->Velocity.Zero();
	OwnerCharacter->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	// �ش� �������� ĳ���� ȸ���ϱ� 
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FRotator TargetRotator = UKismetMathLibrary::FindLookAtRotation(CurrentLocation, TargetLocation);

	OwnerCharacter->SetActorRotation(TargetRotator);

	if (!!GrapplingMontage)
	{
		OwnerCharacter->PlayAnimMontage(GrapplingMontage);
	}
}

void UCGrapplingComponent::End_DoGrappling()
{
	bGrappleEnd = true;
	ResetGrapple();
}



void UCGrapplingComponent::Grapple_1(float InDetaTime)
{
	CheckFalse(bIsGrappling);

	// �̱���� ���� ��ÿ� ������ 
	if (bGrappleEnd == false)
	{
		Begin_DoGrappling();

		// ��ǥ �������� ���� �Ÿ�
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal(); // ���� ����
		FVector NewLocation = CurrentLocation + (Direction * PullSpeed* InDetaTime); // �ӵ� * �ð� = �Ÿ�

		OwnerCharacter->SetActorLocation(NewLocation);

		// ��ǥ ������ �����ߴ��� Ȯ��
		if (FVector::Dist(NewLocation, TargetLocation) < DistanceThreshold)
		{
			End_DoGrappling();
		}
	}
}

void UCGrapplingComponent::Grapple_2(float InDetaTime)
{
	//TODO: õõ�� ���ٰ� ���߿� �ߴ��ϰų� ��ư ���� �ű⼭ ���߰��ϸ�ɵ� 
	if (bGrappleEnd == false)
	{
		FVector NewLocation = FMath::Lerp(OwnerCharacter->GetActorLocation(), TargetLocation, PullSpeed);
		OwnerCharacter->SetActorLocation(NewLocation);

		// ��ǥ ������ �����ߴ��� Ȯ��
		if (FVector::Dist(OwnerCharacter->GetActorLocation(), TargetLocation) < DistanceThreshold)
		{
			// �׷��ø� �Ϸ� ó�� ����
			//bIsGrappling = false;
			bGrappleEnd = true;
		}
	}
}