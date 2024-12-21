#include "Characters/CBaseCharacter.h"

ACBaseCharacter::ACBaseCharacter()
{

}

void ACBaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (OnCharacterLanded.IsBound())
		OnCharacterLanded.Broadcast();

}


