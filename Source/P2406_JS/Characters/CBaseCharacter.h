#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enums/FEnum.h"
#include "Damages/CDamageHandler.h"
#include "GenericTeamAgentInterface.h"
#include "CBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterLanded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterEndDamaged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDowned);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterRaised);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterDead);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCharacterEndDead);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCharacterDead_One, ACharacter*, InInstigator);

UCLASS()
class P2406_JS_API ACBaseCharacter
	: public ACharacter
	, public IGenericTeamAgentInterface

{
	GENERATED_BODY()



protected:
	UPROPERTY(EditDefaultsOnly, Category = "Character ID")
	uint32 UID = UINT32_MAX;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 2;

	UPROPERTY(EditAnywhere, Category = "Group")
	int32 GroupID = 0;

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
	FORCEINLINE uint32 GetUID() const { return UID; }
	FORCEINLINE uint8 GetTeamID() const { return TeamID; }
	FORCEINLINE EEntityGrade GetGrade() const { return Grade; }
	FORCEINLINE int32 GetGroupID() const { return GroupID; }
	FORCEINLINE void SetGroupID(int32 InGroupID) { GroupID = InGroupID; }

public:
	FORCEINLINE bool GetCanAct() const { return bCanAct; }
	FORCEINLINE bool GetExecutable() const { return bExecutable; }

public:
	FORCEINLINE FGenericTeamId GetGenericTeamId() const { return FGenericTeamId(TeamID); }

public:
	ACBaseCharacter();

protected:
	virtual void BeginPlay() override;

public:
	FOnCharacterLanded OnCharacterLanded;
	FOnCharacterDamaged OnCharacterDamaged;
	FOnCharacterEndDamaged OnCharacterEndDamaged;
	FOnCharacterDowned OnCharacterDowned;
	FOnCharacterRaised OnCharacterRaised;
	FOnCharacterDead OnCharacterDead;
	FCharacterDead_One OnCharacterDead_One;
	FOnCharacterEndDead OnCharacterEndDead;

protected:
	virtual void Launch(const struct FHitData& InHitData, const bool bIsGuarding = false) {};

	virtual void Landed(const FHitResult& Hit) override;

	virtual void Play_DamageMontage(const struct FHitData& hitData) {};

public:
	virtual bool IsJumping();

protected:
	virtual void Dead();

	virtual void ResetIgoreCollision();


protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCAirborneComponent* Airborne;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UCConditionComponent* Condition;

//public:
	//struct FDamageData
	//{
	//	float Power;
	//	class ACharacter* Attacker;
	//	class AActor* Causer;

	//	struct FActionDamageEvent* Event;

public:
	struct FDamageData DamageData;

protected:
	ECollisionEnabled::Type CollsionEnabledType;
	bool bShouldCountDownOnLand = false;
	bool bCanAct = true;
	bool bExecutable = true; //TODO : µû·Î »©±â
};
