#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/FEnum.h"
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
	UPROPERTY(EditAnywhere, Category = "Grade")
	EEntityGrade Grade = EEntityGrade::MAX;

protected:
	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DamagedMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* AirborneDamagedMontage;

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DownBeginMontage; 

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DownDamgeMontage; 

	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* RaiseMontage;


	UPROPERTY(EditAnywhere, Category = "Montage")
	class UAnimMontage* DeadMontage;
	
public:
	FORCEINLINE EEntityGrade GetGrade() { return Grade; }

public:
	ACBaseCharacter();


public:
	FOnCharacterLanded OnCharacterLanded;
	FOnCharacterEndDamaged OnCharacterEndDamaged;
	FOnCharacterDowned OnCharacterDowned;
	FOnCharacterRaised OnCharacterRaised;

protected:
	virtual void Launch(const struct FHitData& InHitData, const bool bIsGuarding = false) {};
	
	virtual void Landed(const FHitResult& Hit) override;

	virtual void Play_DamageMontage(const struct FHitData& hitData) {};

protected:
	protected:
	UPROPERTY(VisibleAnywhere, Category ="Components")
	class UCAirborneComponent* Airborne;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCConditionComponent* Condition; 

public:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;
	} DamageData;

protected:
	bool bShouldCountDownOnLand = false;
};
