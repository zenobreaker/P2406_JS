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
//		CLog::Print("Upper Hit");
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

float UCAirborneComponent::GetAddedAirValue(float LaunchPower, AActor* InCauser)
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
			finalLaunchPower = FMath::Clamp(500.0f, LaunchPower * 0.1f, LaunchPower * 0.1f); // 10%��
			CLog::Print("Upper Hit");
		}
		// ���ٸ� ������ ������ 
		//TODO: ������ ���� ó���ؾ��� �ʿ䰡 �ִ�. 
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
	// �����ϸ� ���� ���� ����
	if (bIsAirborne == false)
		return; 

	bIsAirborne = false;
	CLog::Print("Airborne = Landed");
	if (!!Causer)
	{
		//OwnerCharacter->GetCapsuleComponent()->SetCollisionProfileName("Pawn");
		Causer = nullptr;

		//USkeletalMeshComponent* skeletal = CHelpers::GetComponent<USkeletalMeshComponent>(OwnerCharacter);

		//TODO: ���Ⱑ �ٽ� Ȱ��ȭ�Ǹ� ������ �Ǵ°� ����.
		/*if (!!skeletal)
			skeletal->SetCollisionProfileName("Pawn");*/
	}
}

