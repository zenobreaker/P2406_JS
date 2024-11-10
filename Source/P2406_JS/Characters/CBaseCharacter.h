#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLanded);

UCLASS()
class P2406_JS_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ACBaseCharacter();


public:
	FOnCharacterLanded OnCharacterLandedDelegate;

protected:
	virtual void Landed(const FHitResult& Hit) override;


};
