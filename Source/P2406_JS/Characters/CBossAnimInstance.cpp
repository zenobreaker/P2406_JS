#include "Characters/CBossAnimInstance.h"
#include "Global.h"
#include "GameFrameWork/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/CEnemy_AI.h"
#include "Characters/CBoss_AI.h"

#include "Components/CConditionComponent.h"


void UCBossAnimInstance::NativeBeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Condition = FHelpers::GetComponent<UCConditionComponent>(OwnerCharacter);
	ACBoss_AI* boss = Cast<ACBoss_AI>(OwnerCharacter);
	CheckNull(boss);

	REGISTER_EVENT_WITH_REPLACE(boss, OnBossStateChanged, this, UCBossAnimInstance::OnBossStateChanged);
}

void UCBossAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	// Áü¹ú¶ô ¹æÁö 
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	//if (OwnerCharacter->bUseControllerRotationYaw == false)
	//	Direction = 0;

	if (BossState == EBossState::Rotate)
	{
		RotateSpeed = Direction;
		FLog::Print("BANIM Rotate Speed " + FString::SanitizeFloat(RotateSpeed),  36361);
	}
}

void UCBossAnimInstance::OnBossStateChanged(EBossState InType)
{
	BossState = InType; 
}
