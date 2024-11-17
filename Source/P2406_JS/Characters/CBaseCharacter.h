#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLanded);

UCLASS()
class P2406_JS_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()



protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DeadMontage;


public:
	ACBaseCharacter();


public:
	FOnCharacterLanded OnCharacterLandedDelegate;

protected:
	virtual void Landed(const FHitResult& Hit) override;

protected:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} DamageData;
};
