#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "Components/CWeaponComponent.h"
#include "CSkillStructures.generated.h"


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

	UPROPERTY(EditAnywhere, Category = "Info")
	class UAnimMontage* CastingAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Info")
	class UAnimMontage* PlayAnimMontage;
};

USTRUCT()
struct FSkillActionData : public FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float HitDelay;

public:
	void DoAction(class ACharacter* InOwner) override;
	void Destroy_GhostTrail() override;

};


UCLASS()
class P2406_JS_API UCSkillStructures : public UObject
{
	GENERATED_BODY()
};
