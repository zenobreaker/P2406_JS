#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/CSkillAsset.h"
#include "CSkillComponent.generated.h"

UENUM(BlueprintType)
enum class ESkillSlot : uint8
{
	Skill1, Skill2, Skill3, Skill4, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSetSkills, const TArray<class UCActiveSkill*>&, InActiveSkills);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUsed, UCSkillAsset*, InSkillAsset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillSlotsCleared);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSkillExecuted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCurrentSkillEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdatedChargeVisiable, bool, bInVisible);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdatedChargeGauge, float, InValue, float, InMaxValue);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCSkillComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY()
	class UCActiveSkill* CurrentSkill;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSetSkills OnSetSkills;

public:	
	UCSkillComponent();

public:
	FORCEINLINE bool GetIsSkillAction() { return bIsSkillAction; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void ExecuteSkill(int32 InSlot);
	void ReleaseSkill(int32 InSlot);
	void CreateSkillCollision(); 
	void CreateSkillEffect();

	void SetEmptySkillList();
	void SetSkillList(const TArray<class UCActiveSkill*>& InActiveSkills);

private:
	void Update_CheckSkillComplete(float InDeltaTime);
	void Update_SkillCooldown(float InDeltaTime); 

public:
	void BeginSkill(); 
	void EndSkill();


private:
	void HandleChargingSkill(/*class UCActiveSkill* InActiveSkill*/); 
	void HandleChargingSkill_Visible(class UCActvieSkill_Charge* InChargeSkill);
	void HandleChargingSkill_Updated(class UCActvieSkill_Charge* InChargeSkill);

public:
	void OnSkillCasting();
	void OffSkillCasting();
	void OnSkillDoAction(); 
	void OffSkillDoAction();

public:
	UFUNCTION()
	void EndedSkill();

public:
	FOnSkillUsed OnSkillUsed;
	FOnSkillSlotsCleared OnSkillSlotsCleared;
	FOnSkillExecuted OnSkillExecuted;
	FOnCurrentSkillEnded OnCurrentSkillEnded;
	FOnUpdatedChargeVisiable OnUpdatedChargeVisiable;
	FOnUpdatedChargeGauge OnUpdatedChargeGauge;

private:
	bool bIsSkillAction;
	const bool* bCanMove;

private:
	TMap<ESkillSlot, class UCActiveSkill*> SkillSlotTable;
	TArray<UCActiveSkill*> ActiveSkills;

private:
	class ACharacter* OwnerCharacter; 
	class UCSkillManager* SkillManager; 
	class UCMovementComponent* Movement;
};
