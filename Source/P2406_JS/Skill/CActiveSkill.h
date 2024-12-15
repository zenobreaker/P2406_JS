#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CWeaponComponent.h"
#include "Skill/CSkillStructures.h"
#include "CActiveSkill.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCooldownUpdated, float, InDeltaTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoaringBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoaringEnd);
DECLARE_DELEGATE(FSkillAction);

enum class ESkillPhase : int8
{
	Start,
	Begin_Casting,
	Casting,
	End_Casting,
	Begin_Skill,
	Charging,         // 추가: 충전 중
	WaitingForInput,  // 추가: 입력 대기 중
	DoAction_Skill,
	End_Skill,
	Effect,
	Finished,
	Max
};

/// <summary>
/// 실제로 캐릭터가 소지하여 사용할 스킬 정보
/// </summary>
UCLASS(Blueprintable)
class P2406_JS_API UCActiveSkill : public UObject
{
	GENERATED_BODY()

private:
	friend class UCSkillAsset;

public:
	FORCEINLINE bool IsCoolDown() const { return currentCooldown <= 0.0f; }
	FORCEINLINE bool GetIsExecute() const {
		return (currentPhase != ESkillPhase::Max) || (currentPhase != ESkillPhase::Finished);
	}
	FORCEINLINE bool GetIsFinished() const { return currentPhase == ESkillPhase::Finished; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill Info")
	FSkillInfo SkillInfo;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSkillActionData> DoActionDatas;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSkillHitData> HitDatas;

public:
	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSkillCooldownUpdated OnSkillCooldownUpdated; 

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnActionBegin OnActionBegin;

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnActionEnd OnActionEnd;

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSoaringBegin OnSoaringBegin;

	UPROPERTY(BlueprintAssignable, Category = "Skill")
	FOnSoaringEnd OnSoaringEnd;

public:
	virtual void BeginPlay(
		ACharacter* InOwner,
		const TArray<FSkillActionData>& InDoActionDatas,
		const TArray<FSkillHitData>& InHitDatas);

	virtual void Tick(float InDeltaTime);


public:
	void RegisterSkillPhase_Normal();
	virtual void RegisterSkillPhase(ESkillPhase phase, FSkillAction action);

	void ExecuteSkill();
	void EndSkill();

	void CasetingSkill(float InTime);

	UFUNCTION()
	void StartNextPhase();
	virtual void ExecutePhase(ESkillPhase phase);

	void DelayNextData(float InTime);

	void Update_Cooldown(float InDeltaTime);

protected:
	// 바로 다음 ActionData를 처리한다. 
	virtual void DoNextData();

public:
	virtual void Begin_Casting();
	virtual void DoCasting();
	virtual void End_Casting();

	virtual void Begin_Skill();
	virtual void DoAction_Skill();

	virtual void Begin_Charging();
	virtual void Begin_WaitInput();

	virtual void End_Skill();

	virtual void Create_Collision();
	virtual void Create_Effect();

public:
	virtual void OnSkillCasting();
	virtual void OffSkillCasting();

	virtual void OnSkillDoAction();
	virtual void OffSkillDoAction();

protected:
	int32 Index;
	class ACharacter* OwnerCharacter;

	float currentCooldown = 0.0f;
	float currentCastingTime;
	float currentDelay;

	ESkillPhase currentPhase = ESkillPhase::Start;
	TMap<ESkillPhase, FSkillAction> SkillPhaseTable;
};
