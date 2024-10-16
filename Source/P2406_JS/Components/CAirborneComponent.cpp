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

	// 공중 상태가 아니면 띄움
	CheckTrue(bIsAirborne);

	bIsAirborne = true;
	FVector LaunchVelocity = FVector(0, 0, LaunchPower);
	OwnerCharacter->LaunchCharacter(LaunchVelocity, true, true); // 캐릭터를 위로 발사 시킴
}

void UCAirborneComponent::Landed(const FHitResult& Hit)
{
	// 착지하면 공중 상태 헤제
	bIsAirborne = false;
}

