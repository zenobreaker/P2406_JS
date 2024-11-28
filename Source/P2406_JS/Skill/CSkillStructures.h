#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "Components/CWeaponComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CSkillStructures.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionCompleted);

USTRUCT()
struct FSkillInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Info")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(EditAnywhere, Category = "Info")
	FString SkillName = "";

	UPROPERTY(EditAnywhere, Category = "Info")
	float CastingTime = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Info")
	float CoolDown = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Info")
	float Cost = 0.0f;
};

USTRUCT()
struct FSkillActionData : public FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float HitDelay;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* SkillEffect;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* BeginCastingAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* CastingAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EndCastingAnimMontage;

	FOnActionCompleted OnAcctionCompleted;

	//TODO: 스킬 판정 
	//UPROPERTY(EditAnywhere)

public:
	void DoAction(class ACharacter* InOwner) override;

	virtual void Begin_Casting(class ACharacter* InOwner);
	virtual void DoCasting(class ACharacter* InOwner);
	virtual void End_Casting( class ACharacter* InOwner);

	void Destroy_GhostTrail() override;

};


UCLASS()
class P2406_JS_API UCSkillStructures : public UObject
{
	GENERATED_BODY()
};
