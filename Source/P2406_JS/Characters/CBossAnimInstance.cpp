#include "Characters/CBossAnimInstance.h"
#include "Global.h"
#include "GameFrameWork/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/CEnemy_AI.h"
#include "Characters/CBoss_AI.h"

#include "Components/CConditionComponent.h"
#include "Components/CStatComponent.h"


void UCBossAnimInstance::NativeBeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Condition = FHelpers::GetComponent<UCConditionComponent>(OwnerCharacter);
	ACBoss_AI* boss = Cast<ACBoss_AI>(OwnerCharacter);
	CheckNull(boss);

	Feet = FHelpers::GetComponent<UCFeetComponent>(OwnerCharacter); 

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

	ChangeFeet(); 
}

void UCBossAnimInstance::OnBossStateChanged(EBossState InType)
{
	BossState = InType; 
}

void UCBossAnimInstance::ChangeFeet()
{
	bUseFootIK = false; 
	if (Feet != nullptr)
	{
		if (State->IsActionMode() || Condition->GetAirborneCondition() || Condition->GetDownCondition())
			return;

		bUseFootIK = true;

		if (bUseFootIK)
		{
			FeetData = Feet->GetData();
			float offsetDistance = Feet->GetOffsetDistance();
			float leftDistance = FeetData.LeftDistance.X;
			float rightDistance = FeetData.RightDistance.X;

			if (FMath::IsNearlyZero(offsetDistance))
				offsetDistance = 1.0f;
			
			LeftIKValue = GetIKValue(leftDistance, offsetDistance);
			RightIKValue = GetIKValue(rightDistance, offsetDistance);

			//FLog::Print("Left IK VALUE : " + FString::SanitizeFloat(LeftIKValue), 92525);
			//FLog::Print("Right IK VALUE : " + FString::SanitizeFloat(RightIKValue), 92526);
		}
	}
}

float UCBossAnimInstance::GetIKValue(float InValue1, float InValue2)
{

	if (FMath::IsNearlyZero(InValue1, 0.01f))
		return 1.0f; 

	if (FMath::IsNearlyZero(InValue2, 0.01f))
		return 1.0f;

	float retrunValue = FMath::Abs(InValue1) / FMath::Abs(InValue2);
	retrunValue = FMath::Clamp(retrunValue, 0.0f, 1.0f);
	return retrunValue;
}
