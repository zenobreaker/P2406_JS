#include "Components/CAirborneComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

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


void UCAirborneComponent::LaunchIntoAir(float LaunchPower)
{
	CheckNull(movement);
	CheckNull(OwnerCharacter);

	float finalLaunchPower = 0.0f; 
	// ���� ���°� �ƴϸ� ���
	if (bIsAirborne == false && LaunchPower > 0.0f)
	{
		bIsAirborne = true;
		finalLaunchPower = LaunchPower;
		CLog::Print("Upper Hit");
	}
	else if(OwnerCharacter->GetCharacterMovement()->IsFalling() == true)
	{
		if (LaunchPower > 0.0f)
		{
			LaunchPower = LaunchPower * 0.01f;
			finalLaunchPower = LaunchPower;
		}
		else
		{
			finalLaunchPower = 500.0f;
			CLog::Print("Air Hit");
			FVector Impulse = FVector(0, 0, finalLaunchPower);
		}
	}

	FVector LaunchVelocity = FVector(0, 0, finalLaunchPower);
	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true); // ĳ���͸� ���� �߻� ��Ŵ
}

void UCAirborneComponent::Landed(const FHitResult& Hit)
{
	// �����ϸ� ���� ���� ����
	bIsAirborne = false;
	CLog::Print("Airborne = Landed");
}

