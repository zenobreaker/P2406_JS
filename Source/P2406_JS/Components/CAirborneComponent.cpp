#include "Components/CAirborneComponent.h"
#include "Global.h"
#include "Characters/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

//TODO: 이거 상태 전용으로 구성한 컴포넌트인데.. 상태용으로 만든게 있어서 이걸 유지할지 고민해야겟다.
UCAirborneComponent::UCAirborneComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;
}


void UCAirborneComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	CheckNull(OwnerCharacter);
	
	ACBaseCharacter* BC = Cast<ACBaseCharacter>(OwnerCharacter);
	if(!!BC)
		BC->OnCharacterLanded.AddDynamic(this, &UCAirborneComponent::Landed);

	movement = FHelpers::GetComponent<UCharacterMovementComponent>(OwnerCharacter);
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
//		FLog::Print("Upper Hit");
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

float UCAirborneComponent::Calc_AirborenValue(float LaunchPower, AActor* InCauser)
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
			finalLaunchPower = FMath::Clamp(LaunchPower, LaunchPower * 0.1f, LaunchPower * 5.0f); // 10%로
			//FLog::Print("Upper Hit");
		}
		// 없다면 임의의 값으로 
		//TODO: 임의의 값을 처리해야할 필요가 있다. 
		else
		{
			finalLaunchPower = 250.0f;
			//FLog::Print("Upper Added Hit ");
		}

	}

	return finalLaunchPower;
}

void UCAirborneComponent::Landed()
{
	// 착지하면 공중 상태 헤제
	if (bIsAirborne == false)
		return; 

	bIsAirborne = false;
	//FLog::Print("Airborne = Landed");
	if (!!Causer)
	{
		Causer = nullptr;
	}
}

