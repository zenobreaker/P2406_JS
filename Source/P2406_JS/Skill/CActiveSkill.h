#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CWeaponComponent.h"
#include "Skill/CSkillStructures.h"
#include "CActiveSkill.generated.h"


enum class ESkillPhase : int8
{
	Start, 
	Begin_Casting,
	Casting, 
	End_Casting,
	Execution, 
	Effect, 
	Finished, 
	Max 
};

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
	FORCEINLINE bool GetIsFinished() { return currentPhase == ESkillPhase::Finished; }

public:
	void BeginPlay(
		ACharacter* InOwner,
		const TArray<FSkillActionData>& InDoActionDatas, 
		const TArray<FHitData>& InHitDatas);

	void Tick(float InDeltaTime);

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
	void StartPhase(ESkillPhase phase);

	void DelayNextData(float InTime);

	void DoActionData();

	virtual void Begin_Casting(); 
	virtual void DoCasting();
	virtual void End_Casting();

	virtual void Begin_Skill(); 
	virtual void End_Skill();
	
	virtual void Create_Effect();

private:
	int32 Index; 
	class ACharacter* OwnerCharacter; 
	bool isCasting = false;
	bool castingComplete = false;

	float currentCooldown;
	float currentCastingTime; 
	float currentDelay;

	ESkillPhase currentPhase = ESkillPhase::Start;
};
