#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "Components/CWeaponComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CSkillStructures.generated.h"


USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Info")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,  Category = "Info")
	int32 SkillID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	UTexture2D* SkillIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FString SkillName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float CastingTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float CoolDown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
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
	class USoundWave* Sound;

	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* SkillEffect;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* BeginCastingAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* CastingAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* EndCastingAnimMontage;

	//스킬 판정 
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACSkillCollision> SkillCollisionClass;


public:
	void DoAction(class ACharacter* InOwner) override;
	virtual void Create_SkillCollision(class ACharacter* InOwner, const TArray<FSkillHitData>& InHitDatas);
	virtual void Create_SkillEffect(class ACharacter* InOwner);

	// 이건 좀 특수한 경우인데 다음에 만들면 복잡하게 말고 섹션을 통하는 것도 조케따
	virtual void PlaySection_SkillCastingMontage(class ACharacter* InOwner ,float InPlayRate = 1.0f, FName StartSectionName = EName::None);
	virtual void PlaySecion_SkillActionMontage(class ACharacter* InOwner, float InPlayRate = 1.0f, FName StartSectionName = EName::None);

	virtual void Begin_Casting(class ACharacter* InOwner, bool InLoop = false);
	virtual void DoCasting(class ACharacter* InOwner, bool InLoop = false);
	virtual void End_Casting( class ACharacter* InOwner, bool InLoop = false);


	void PlaySoundWave(class ACharacter* InOwner);

	void Destroy_GhostTrail() override;

private:
	void SkillPlayMontage(class ACharacter* InOwner, class UAnimMontage* InMontage, bool InLoop);
};


USTRUCT()
struct FSkillHitData : public FHitData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float CollisionInterval;
};


UCLASS()
class P2406_JS_API UCSkillStructures : public UObject
{
	GENERATED_BODY()
};
