#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CWeaponComponent.h"
#include "Enums/FEnum.h"
#include "Skill/CSkillStructures.h"
#include "CActiveSkill.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillCooldownUpdated, float, InDeltaTime);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnActionEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoaringBegin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSoaringEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkilCastingCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillEnded);
DECLARE_DELEGATE(FSkillAction);


/// <summary>
/// 실제로 캐릭터가 소지하여 사용할 스킬 정보
/// </summary>
UCLASS(Abstract, Blueprintable, BlueprintType)
class P2406_JS_API UCActiveSkill : public UObject
{
	GENERATED_BODY()

private:
	friend class UCSkillAsset;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	FSkillInfo SkillInfo;

public:
	FORCEINLINE int32 GetSkillID() const { return SkillInfo.SkillID; }
	FORCEINLINE float GetCooldown() const { return SkillInfo.CoolDown; }

	FORCEINLINE bool IsCooldown() const { return currentCooldown <= 0.0f; }
	FORCEINLINE bool GetIsExecute() const {
		return (CurrentPhase != ESkillPhase::Max) && (CurrentPhase != ESkillPhase::Finished);
	}
	FORCEINLINE bool GetIsFinished() const { return CurrentPhase == ESkillPhase::Finished; }
	FORCEINLINE bool GetCompleteCasting() const { return currentCastingTime >= SkillInfo.CastingTime; }

protected:
	struct FSkillPhase
	{
		TArray<FSkillPhaseData> PhaseDatas;
		TFunction<void()> PhaseFunction = nullptr; // 실행할 페이즈 함수

		FSkillPhase() = default;
		FSkillPhase(const TArray<FSkillPhaseData>& InDatas, TFunction<void()> InFunction)
			: PhaseDatas(InDatas), PhaseFunction(InFunction) {}

	};


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
	virtual void BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData);

	virtual void Tick(float InDeltaTime);


protected:
	TFunction<void()> GetPhaseFunction(ESkillPhase InPhase);
	virtual void DefineSkillPhases() {};
	void SetupDefaultSkillPhase();
	virtual void AssignSkillPhase(ESkillPhase InPhase, TFunction<void()> InFunc);

public:
	virtual void ExecuteSkill();
	// 해당 스킬을 뗐을 때 발동
	virtual void ReleaseSkill() {}
	// 강제로 스킬을 종료한다. 
	void EndSkill();

	void Update_Cooldown(float InDeltaTime);

	void CastingSkill(float InTime);

protected:
	virtual ESkillPhase GetNextFlowPhase(ESkillPhase InPhase);
	virtual void ExecutePhase(ESkillPhase InPhase);
	void ExecutePhaseData(ESkillPhase InPhase);
public:
	UFUNCTION()
	void OnChangeNextSkillPhase(ESkillPhase InPhase);

	//이건 왜 인지 모르겠는데 적용이 안된다. 사연을 알면 좋은데 모르니.. 
	UFUNCTION()
	void OnMontageEnded(class UAnimMontage* Montage, bool bInterrupted);


public:
	virtual void Begin_Casting() {}
	virtual void End_Casting() {}

	virtual void Begin_Skill() {}
	virtual void End_Skill() {}


	virtual void Begin_WaitInput() {}

public:
	virtual void Create_SkillEffect() {}
	virtual void Create_Collision() {}

	virtual void OnSkillCasting() {}
	virtual void OffSkillCasting() {}

	virtual void OnSkillDoAction() {}
	virtual void OffSkillDoAction() {}

protected:
	FOnSkilCastingCompleted OnSkillCastingCompleted;
	FOnSkillEnded OnSkillEnded;

protected:
	FSkillFlowData SkillFlowData;

protected:
	class ACharacter* OwnerCharacter;

protected:
	bool bIsAction = false; 
	float currentCooldown = 0.0f;
	float currentCastingTime;
	float currentDelay;

	ESkillPhase CurrentPhase = ESkillPhase::Max;
	// 해당 페이즈에서 처리될 내용 및 다음 페이즈로 보내든가의 조건 
	TMap<ESkillPhase, FSkillPhase> SkillPhaseTable;

protected:
	TMap<ESkillPhase, TArray<FSkillPhaseData>> PhaseDataTable;

};
