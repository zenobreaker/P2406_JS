#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "IDamageState.h"
#include "CDamageHandler.generated.h"


UENUM()
enum class EDamageType : uint8
{
	Normal = 0, Launch, Down, Airbone, Max,
};

USTRUCT()
struct FDamageAnimData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere);
	class UAnimMontage* Montage = nullptr;

	UPROPERTY(EditAnywhere);
	float PlayRate = 1.0f; 
};

UCLASS(Blueprintable)
class P2406_JS_API UCDamageHandler : public UObject
{
	GENERATED_BODY()

private: 
	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<EDamageType, class UAnimMontage*> DamageMontages;

public:
	void BeginPlay(class ACharacter* InCharacter);

public:
	void ApplyDamage(const struct FHitData& InHitData);

protected:
	class ACharacter* OwnerCharacter;

protected:
	TMap<EDamageType, IIDamageState*> DamageStateTable; 
};
