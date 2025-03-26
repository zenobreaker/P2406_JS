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
	{
		BC->OnCharacterLanded.AddDynamic(this, &UCAirborneComponent::Landed);
		REGISTER_EVENT_WITH_REPLACE(BC, OnCharacterEndDamaged, this, UCAirborneComponent::OnCharacterEndDamaged);
	}

	OriginGravity = OwnerCharacter->GetCharacterMovement()->GravityScale; 
}



float UCAirborneComponent::Calc_AirborneValue(float InAerial, AActor* InCauser)
{
	float finalLaunchPower = 0.0f;
	if (bIsAirborne == false && InAerial > 0.0f)
	{
		bIsAirborne = true;
		finalLaunchPower = InAerial;
	}
	else if(bIsAirborne == true)
	{
		// �̹� ���� ���� �ִٸ� 
		if (InAerial > 0.0f)
			finalLaunchPower = FMath::Clamp(InAerial, InAerial * 0.1f, InAerial * 5.0f); // 10%��
		else
		{
			OwnerCharacter->GetCharacterMovement()->Velocity.Z = 0.0f;
			OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;

			return 0.0f;
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
}

void UCAirborneComponent::OnCharacterEndDamaged()
{
	CheckFalse(bIsAirborne);

	OwnerCharacter->GetCharacterMovement()->GravityScale = OriginGravity;
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