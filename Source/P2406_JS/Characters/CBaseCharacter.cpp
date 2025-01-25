#include "Characters/CBaseCharacter.h"
#include "Global.h"

ACBaseCharacter::ACBaseCharacter()
{

}

void ACBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	DYNAMIC_EVENT_CALL(OnCharacterLanded);
}

void ACBaseCharacter::Dead()
{
	DYNAMIC_EVENT_CALL(OnCharacterDead);
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnCharacterDead_One, this);
}




