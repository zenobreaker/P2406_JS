#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "GameFrameWork/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/CPlayer.h"
#include "Components/CConditionComponent.h"
#include "Components/CGrapplingComponent.h"
#include "Weapons/CSubAction.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = FHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	State = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Skill = FHelpers::GetComponent<UCSkillComponent>(OwnerCharacter); 
	Grapple = FHelpers::GetComponent<UCGrapplingComponent>(OwnerCharacter);
	Condition = FHelpers::GetComponent<UCConditionComponent>(OwnerCharacter);

	if (!!Weapon)
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCAnimInstance::OnWeaponTypeChanged);

	if (!!State)
		State->OnStateTypeChanged.AddDynamic(this, &UCAnimInstance::OnStateTypeChanged);

	ACBaseCharacter* baseCharacter = Cast<ACBaseCharacter>(OwnerCharacter);
	if(!!baseCharacter)
	{
		baseCharacter->OnCharacterDowned.AddDynamic(this, &UCAnimInstance::OnCharacterDowned);

		baseCharacter->OnCharacterRaised.AddDynamic(this, &UCAnimInstance::OnCharacterRaised);
	}
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();

	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	// 애님인스턴스에서 처리함 .
	if (Skill != nullptr)
	{
		bSkillSoaring = Skill->GetSkillSoaring(); 
	}

	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling(); /*&& bSkillSoaring == false;*/
	//FLog::Print("Anim bFalling : " + FString::FromInt(bFalling));
	bIsAirborneHit = bFalling && StateType == EStateType::Damaged;
	
	if (!!Condition)
		bDown = Condition->GetDownCondition();

	CheckNull(Grapple);
	bGrappling = Grapple->GetGrappling();
	
	CheckNull(Weapon);
	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming = true;
		bBow_Aiming &= WeaponType == EWeaponType::Bow;
		bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}
}

void UCAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}

void UCAnimInstance::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	StateType = InNewType;
}

void UCAnimInstance::OnCharacterLanded()
{
	State->SetIdleMode();
}

void UCAnimInstance::OnCharacterDowned()
{
	DownBlendValue = 0.0f;
}

void UCAnimInstance::OnCharacterRaised()
{
	DownBlendValue = 100.0f;
}

