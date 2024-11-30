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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSkillUsed, UCSkillAsset*, InSkillAsset);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCSkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSkillComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FOnSkillUsed OnSkillUsed;

	void ExecuteSkill(int32 InSlot);
	void CreateSkillCollision(); 
	void EndSkill();

	void SetSkillList(const TArray<class UCActiveSkill*>& InSkills);

private:
	class ACharacter* OwnerCharacter; 
	TMap<ESkillSlot, class UCActiveSkill*> SkillTable;

private:
	UPROPERTY()
	class UCActiveSkill* Skills[(int32)ESkillSlot::Max];

	UPROPERTY()
	class UCActiveSkill* CurrentSkill; 

	TArray<UCActiveSkill*> ActiveSkills;

};
