#include "Characters/CBaseCharacter.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CConditionComponent.h"

ACBaseCharacter::ACBaseCharacter()
{

}

void ACBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	DYNAMIC_EVENT_CALL(OnCharacterLanded);
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
}




