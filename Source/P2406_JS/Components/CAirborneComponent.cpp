#include "Components/CAirborneComponent.h"
#include "Global.h"
#include "Characters/CBaseCharacter.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"

//TODO: �̰� ���� �������� ������ ������Ʈ�ε�.. ���¿����� ����� �־ �̰� �������� ����ؾ߰ٴ�.
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
//	// ���� ���°� �ƴϸ� ���
//	if (bIsAirborne == false && LaunchPower > 0.0f)
//	{
//		bIsAirborne = true;
//		finalLaunchPower = LaunchPower;
//		// ��� �� �ݸ��� ���¸� �����ؼ� �ֺ� �浹ü ���� ������ 
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
//	// ���� ������ �� ó�� 
//	//else if(OwnerCharacter->GetCharacterMovement()->IsFalling() == true)
//	else if (bIsAirborne == true)
//	{
//		// �̹� ���� ���� �ִٸ� 
//		if (LaunchPower > 0.0f)
//		{
//			finalLaunchPower = FMath::Clamp(500.0f, LaunchPower * 0.1f, LaunchPower * 0.1f); // 10%��
//		}
//		// ���ٸ� ������ ������ 
//		//TODO: ������ ���� ó���ؾ��� �ʿ䰡 �ִ�. 
//		else
//		{
//			finalLaunchPower = 500.0f;
//		}
//	}
//
//	FVector LaunchVelocity = FVector(x, y, finalLaunchPower);
//	OwnerCharacter->LaunchCharacter(LaunchVelocity, false, true); // ĳ���͸� ���� �߻� ��Ŵ
//}

float UCAirborneComponent::Calc_AirborenValue(float LaunchPower, AActor* InCauser)
{
	float finalLaunchPower = 0.0f;
	if (bIsAirborne == false && LaunchPower > 0.0f)
	{
		bIsAirborne = true;
		finalLaunchPower = LaunchPower;
		// ��� �� �ݸ��� ���¸� �����ؼ� �ֺ� �浹ü ���� ������ 
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
		// �̹� ���� ���� �ִٸ� 
		if (LaunchPower > 0.0f)
		{
			finalLaunchPower = FMath::Clamp(LaunchPower, LaunchPower * 0.1f, LaunchPower * 5.0f); // 10%��
			//FLog::Print("Upper Hit");
		}
		// ���ٸ� ������ ������ 
		//TODO: ������ ���� ó���ؾ��� �ʿ䰡 �ִ�. 
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
	// �����ϸ� ���� ���� ����
	if (bIsAirborne == false)
		return; 

	bIsAirborne = false;
	//FLog::Print("Airborne = Landed");
	if (!!Causer)
	{
		Causer = nullptr;
	}
}

