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
	CheckFalse(LaunchPower > 0.0f);
	CheckNull(movement);
	CheckNull(OwnerCharacter);

	// ���� ���°� �ƴϸ� ���
	CheckTrue(bIsAirborne);

	bIsAirborne = true;
	FVector LaunchVelocity = FVector(0, 0, LaunchPower);
	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true); // ĳ���͸� ���� �߻� ��Ŵ
}

void UCAirborneComponent::Landed(const FHitResult& Hit)
{
	// �����ϸ� ���� ���� ����
	bIsAirborne = false;
}

