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

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSkillComponent();

public:
	FORCEINLINE bool GetIsSkillAction() { return bIsSkillAction; }
	FORCEINLINE bool GetSkillSoaring() { return bIsSkillSoaring; }

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSetSkills OnSetSkills;
	FOnSkillUsed OnSkillUsed;
	FOnSkillSlotsCleared OnSkillSlotsCleared;
	FOnSkillExecuted OnSkillExecuted;

public:
	void ExecuteSkill(int32 InSlot);
	void ReleaseSkill(int32 InSlot);
	void CreateSkillCollision(); 
	void CreateSkillEffect();

	void SetEmptySkillList();
	void SetSkillList(const TArray<class UCActiveSkill*>& InActiveSkills);
private:
	UFUNCTION()
	void OnSkillSoaring();
	UFUNCTION()
	void OffSkillSoaring();

private:
	void Update_CheckSkillComplete(float InDeltaTime);
	void Update_SkillCooldown(float InDeltaTime); 

public:
	void BeginSkill(); 
	void EndSkill();
	void OnSkillCasting();
	void OffSkillCasting();
	void OnSkillDoAction(); 
	void OffSkillDoAction();


private:
	class ACharacter* OwnerCharacter; 
	TMap<ESkillSlot, class UCActiveSkill*> SkillSlotTable;

	bool bIsSkillAction = false; 
	bool bIsSkillSoaring = false; 

private:
	//UPROPERTY()
	//class UCActiveSkill* Skills[(int32)ESkillSlot::Max];

	UPROPERTY()
	class UCActiveSkill* CurrentSkill; 

	TArray<UCActiveSkill*> ActiveSkills;


private:
	class UCSkillManager* SkillManager; 
	class UCMovementComponent* Movement;
};
