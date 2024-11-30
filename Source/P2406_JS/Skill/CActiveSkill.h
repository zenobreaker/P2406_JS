#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CWeaponComponent.h"
#include "Skill/CSkillStructures.h"
#include "CActiveSkill.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionCompleted);

enum class ESkillPhase : int8
{
	Start,
	Begin_Casting,
	Casting, 
	End_Casting,
	Begin_Skill,
	DoAction_Skill,
	End_Skill,
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
	FORCEINLINE bool GetIsExecute() {
		return (currentPhase != ESkillPhase::Start &&
			currentPhase != ESkillPhase::Finished && 
			currentPhase != ESkillPhase::Max);}
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

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnActionCompleted OnActionCompleted;

public:
	void ExecuteSkill();
	void EndSkill();

	void CasetingSkill(float InTime);
	void StartNextPhase();
	void ExecutePhase(ESkillPhase phase);

	void DelayNextData(float InTime);
	
public:
	virtual void Begin_Casting(); 
	virtual void DoCasting();
	virtual void End_Casting();

	virtual void Begin_Skill(); 
	virtual void DoAction_Skill();
	virtual void End_Skill();
	
	virtual void Create_Collision();

private:
	int32 Index; 
	class ACharacter* OwnerCharacter; 

	float currentCooldown;
	float currentCastingTime; 
	float currentDelay;

	ESkillPhase currentPhase = ESkillPhase::Start;
};
