#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "CActiveSkill_Stance.generated.h"

/// <summary>
///  이 스킬은 특정 입력이 될 때 까지 특정 데이터 상태에서 대기 하는 특징을 가진다.
/// </summary>
UCLASS()
class P2406_JS_API UCActiveSkill_Stance : public UCActiveSkill
{
	GENERATED_BODY()

public:
	virtual void BeginPlay(
		ACharacter* InOwner,
		const TArray<FSkillActionData>& InDoActionDatas,
		const TArray<FSkillHitData>& InHitDatas) override;

	virtual void Tick(float InDeltaTime) override;

protected:
	// 입력 받을 키 세팅 
	void SetSkillKey(EKeys InTargetKey); 

	// 특정 키 입력 
	virtual void Input_AnyKey(); 
	

	virtual void Begin_WaitInput() override;

private:
	void RegisterSkillPhase_Stance();

private:
	// 이 스킬이 세팅된 키보드 키코드 
	EKeys SkillKey; 
	// 공격 키 
	EKeys AttackKey;
};
