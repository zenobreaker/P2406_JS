#include "Characters/CBaseCharacter.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Components/CConditionComponent.h"

ACBaseCharacter::ACBaseCharacter()
{

}

void ACBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	CollsionEnabledType = GetCapsuleComponent()->GetCollisionEnabled();


	if (!!Condition)
	{
		OnCharacterLanded.AddDynamic(Condition, &UCConditionComponent::RemoveAirborneCondition);
	}
}

void ACBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	DYNAMIC_EVENT_CALL(OnCharacterLanded);
	ResetIgoreCollision();
}

bool ACBaseCharacter::IsJumping()
{
	bool isJumping = true;
	isJumping &= GetCharacterMovement()->IsFalling();
	isJumping |= GetCharacterMovement()->IsFlying();

	if (Condition != nullptr)
	{
		isJumping &= Condition->GetAirborneCondition() == false;

	}


	return isJumping;
}

void ACBaseCharacter::Dead()
{
	DYNAMIC_EVENT_CALL(OnCharacterDead);
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnCharacterDead_One, this);
	//ResetIgoreCollision();
}

void ACBaseCharacter::ResetIgoreCollision()
{
	//CollsionEnabled =GetCapsuleComponent()->GetCollisionEnabled();
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	GetCapsuleComponent()->SetCollisionEnabled(CollsionEnabledType);
	//GetCapsuleComponent()->IgnoreActorWhenMoving(DamageData.Attacker, false);
}




