#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLanded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterEndDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDowned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterRaised);

UCLASS()
class P2406_JS_API ACBaseCharacter : public ACharacter
{
	GENERATED_BODY()



protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* AirborneDamagedMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DownMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DeadMontage;
	
public:
	ACBaseCharacter();


public:
	FOnCharacterLanded OnCharacterLanded;
	FOnCharacterEndDamaged OnCharacterEndDamaged;
	FOnCharacterDowned OnCharacterDowned;
	FOnCharacterRaised OnCharacterRaised;
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
