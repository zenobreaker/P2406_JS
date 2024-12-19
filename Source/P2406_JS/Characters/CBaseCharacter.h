#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLanded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterEndDamaged);

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
	class UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DownedMontage;  // 다운 상태 애니메이션

public:
	ACBaseCharacter();


public:
	FOnCharacterLanded OnCharacterLandedDelegate;
	FOnCharacterEndDamaged OnCharacterEndDamaged;

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
