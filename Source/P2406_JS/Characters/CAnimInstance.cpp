#include "Characters/CAnimInstance.h"
#include "Global.h"
#include "GameFrameWork/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Characters/CBaseCharacter.h"
#include "Characters/CPlayer.h"
#include "Characters/IGuardable.h"

#include "Components/CConditionComponent.h"
#include "Components/CGrapplingComponent.h"
#include "Components/CGuardComponent.h"
#include "Components/CParkourComponent.h"
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
	Feet = FHelpers::GetComponent<UCFeetComponent>(OwnerCharacter); 
	Parkour = FHelpers::GetComponent<UCParkourComponent>(OwnerCharacter);

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

	if (Cast<ACPlayer>(OwnerCharacter) != nullptr)
		bPlayer = true;
	else
		bPlayer = false; 
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	CheckNull(OwnerCharacter);

	Speed = OwnerCharacter->GetVelocity().Size2D();
	// 짐벌락 방지 
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);
	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation, delta, DeltaSeconds, 25);
	Direction = PrevRotation.Yaw;

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	if (OwnerCharacter->bUseControllerRotationYaw == false)
		Direction = 0;

	// Falling Attack
	ChangeFallingAttack();

	// Falling 
	ChangeFalling(); 
	
	// Airborne
	ChangeAirborne(); 
	
	// Down 
	ChangeDown(); 

	// Grappling 
	ChangeGrappling(); 

	// Bow Aim
	ChangeBowAiming(); 

	// Gaurd 
	ChangeGuardState();

	//IK
	ChangeFeet(); 
}

//-----------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------

void UCAnimInstance::ChangeGuardState()
{
	// 내가 가져올 때 인터페이스에서 가져오고 거기서 값을 그걸로 바꾼다. 
	UCGuardComponent* guard= FHelpers::GetComponent<UCGuardComponent>(OwnerCharacter);
	CheckNull(guard);

	bGuarding = guard->GetGuarding();
}

void UCAnimInstance::ChangeFeet()
{
	bUseFootIK = false;
	if (!!Feet)
	{
		bUseFootIK = true;

		if (!!Parkour)
			bUseFootIK = Parkour->IsDoing() == false;

		if(bUseFootIK)
			FeetData = Feet->GetData(); 
	}
}

void UCAnimInstance::ChangeFallingAttack()
{
	CheckNull(Weapon);

	/*Weapon->PlayFallingAttackMontage();*/

	/*if(bPlayer)
	FLog::Print("Falling Attack " + FString::FromInt(Weapon->IsFallingAttack()), 2424);*/

	bFallingAttack = Weapon->IsFallingAttack();
}

void UCAnimInstance::ChangeFalling()
{
	bFalling = OwnerCharacter->GetCharacterMovement()->IsFalling(); 

	bFalling &= bFallingAttack == false;
}

void UCAnimInstance::ChangeBowAiming()
{
	CheckNull(Weapon);
	if (!!Weapon->GetSubAction())
	{
		bBow_Aiming = true;
		bBow_Aiming &= WeaponType == EWeaponType::Bow;
		bBow_Aiming &= Weapon->GetSubAction()->GetInAction();
	}
}

void UCAnimInstance::ChangeGrappling()
{
	CheckNull(Grapple);
	bGrappling = Grapple->GetGrappling();

}

void UCAnimInstance::ChangeAirborne()
{
	bIsAirborneHit = bFalling && StateType == EStateType::Damaged;
}

void UCAnimInstance::ChangeDown()
{
	CheckNull(Condition);

	bDown = Condition->GetDownCondition();
}

