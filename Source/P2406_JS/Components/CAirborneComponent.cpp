#include "Components/CAirborneComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

UCAirborneComponent::UCAirborneComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
}


void UCAirborneComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	//OwnerCharacter->Land.AddDynamic(this, &UCAirborneComponent::Landed);
	movement = CHelpers::GetComponent<UCharacterMovementComponent>(OwnerCharacter);
}

//
//void UCAirborneComponent::LaunchIntoAir(float LaunchPower, AActor* InCauser)
//{
//	CheckNull(movement);
//	CheckNull(OwnerCharacter);
//
//	float finalLaunchPower = 0.0f;
//	// 공중 상태가 아니면 띄움
//	if (bIsAirborne == false && LaunchPower > 0.0f)
//	{
//		bIsAirborne = true;
//		finalLaunchPower = LaunchPower;
//		// 띄울 때 콜리전 상태를 변경해서 주변 충돌체 영향 없도록 
//		if (!!InCauser)
//		{
//			Causer = InCauser;
//			OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName("NoPawn");
//			USkeletalMeshComponent* skeletal = CHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);
//
//			if (!!skeletal)
//				skeletal->SetCollisionProfileName("NoPawn");
//		}
//
//		CLog::Print("Upper Hit");
//	}
//	// 공중 상태일 때 처리 
//	//else if(OwnerCharacter->GetCharacterMovement()->IsFalling() == true)
//	else if (bIsAirborne == true)
//	{
//		// 이미 띄우는 힘이 있다면 
//		if (LaunchPower > 0.0f)
//		{
//			finalLaunchPower = FMath::Clamp(500.0f, LaunchPower * 0.1f, LaunchPower * 0.1f); // 10%로
//		}
//		// 없다면 임의의 값으로 
//		//TODO: 임의의 값을 처리해야할 필요가 있다. 
//		else
//		{
//			finalLaunchPower = 500.0f;
//		}
//	}
//
//	FVector LaunchVelocity = FVector(x, y, finalLaunchPower);
//	OwnerCharacter->LaunchCharacter(LaunchVelocity, false, true); // 캐릭터를 위로 발사 시킴
//}

float UCAirborneComponent::GetAddedAirValue(float LaunchPower, AActor* InCauser)
{
	float finalLaunchPower = 0.0f;
	if (bIsAirborne == false && LaunchPower > 0.0f)
	{
		bIsAirborne = true;
		finalLaunchPower = LaunchPower;
		// 띄울 때 콜리전 상태를 변경해서 주변 충돌체 영향 없도록 
		if (!!InCauser)
		{
			Causer = InCauser;
			//OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName("NoPawn");
			//USkeletalMeshComponent* skeletal = CHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);

			//if (!!skeletal)
			//	skeletal->SetCollisionProfileName("NoPawn");
		}

	}
	else if(bIsAirborne == true)
	{
		// 이미 띄우는 힘이 있다면 
		if (LaunchPower > 0.0f)
		{
			finalLaunchPower = FMath::Clamp(500.0f, LaunchPower * 0.1f, LaunchPower * 0.1f); // 10%로
			CLog::Print("Upper Hit");
		}
		// 없다면 임의의 값으로 
		//TODO: 임의의 값을 처리해야할 필요가 있다. 
		else
		{
			finalLaunchPower = 500.0f;
			CLog::Print("Upper Added Hit ");
		}

	}

	return finalLaunchPower;
}

void UCAirborneComponent::Landed(const FHitResult& Hit)
{
	// 착지하면 공중 상태 헤제
	if (bIsAirborne == false)
		return; 

	bIsAirborne = false;
	CLog::Print("Airborne = Landed");
	if (!!Causer)
	{
		//OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
		Causer = nullptr;

		//USkeletalMeshComponent* skeletal = CHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);

		//TODO: 여기가 다시 활성화되면 문제가 되는거 같다.
		/*if (!!skeletal)
			skeletal->SetCollisionProfileName("Pawn");*/
	}
}

