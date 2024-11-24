#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CSkillStructures.generated.h"
#include <Components/CWeaponComponent.h>


USTRUCT()
struct FSkillInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Info")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(EditAnywhere, Category = "Info")
	FName SkillName;

	UPROPERTY(EditAnywhere, Category = "Info")
	float CastringTime;

	UPROPERTY(EditAnywhere, Category = "Info")
	float CoolDown;

	UPROPERTY(EditAnywhere, Category = "Info")
	float Cost;
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
