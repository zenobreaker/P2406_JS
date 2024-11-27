#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CWeaponComponent.h"
#include "Skill/CSkillStructures.h"
#include "CActiveSkill.generated.h"


enum class ESkillPhase { Start, Windup, Execution, Recovery, Finished };

/// <summary>
/// 실제로 캐릭터가 소지하여 사용할 스킬 정보
/// </summary>
UCLASS()
class P2406_JS_API UCActiveSkill : public UObject
{
	GENERATED_BODY()

private:
	friend class UCSkillAsset;

public:
	FORCEINLINE float GetCoolDown() { return SkillInfo.CoolDown; }
	FORCEINLINE bool GetIsCasting() { return isCasting; }

public:
	void BeginPlay(
		ACharacter* InOwner,
		const TArray<FSkillActionData>& InDoActionDatas, 
		const TArray<FHitData>& InHitDatas);

private: 
	UPROPERTY(EditAnywhere, Category = "Skill Info")
	FSkillInfo SkillInfo;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSkillActionData> DoActionDatas;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FHitData> HitDatas;


public:
	void ExecuteSkill();

	void CasetingSkill(float InTime);

	void StartNextPhase();

	void EndSkill(); 

private:
	int32 Index; 
	class ACharacter* OwnerCharacter; 
	bool isCasting = false;
	bool castingComplete = false;

	float currentCooldown;
	float currentCastingTime; 

	ESkillPhase currentPhase = ESkillPhase::Start;
};
