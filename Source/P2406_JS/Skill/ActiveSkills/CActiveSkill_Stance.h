#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "Skill/Skill_Interface/InputableSkill.h"
#include "CActiveSkill_Stance.generated.h"

//  이 스킬은 특정 입력이 될 때 까지 특정 데이터 상태에서 대기 하는 특징을 가진다.
UCLASS()
class P2406_JS_API UCActiveSkill_Stance 
	: public UCActiveSkill
	, public IInputableSkill
{
	GENERATED_BODY()


public:
	virtual void BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData) override;

protected:
	// 입력 받을 키 세팅 
	void SetInputKey(FKey InTargetKey); 

	// 특정 키 입력 
	virtual void Input_AnyKey(); 
	
protected:
	virtual void DefineSkillPhases() override;

	void PressKey() override;
	void ReleaseKey() override;

	virtual void OnPressedKey() {};
	virtual void OnReleasedKey() {};

protected: 
	bool CheckPressedSetKey(FKey InKey);
	bool CheckPressedSetKey(); 
	bool CheckReleasedSetKey(FKey InKey);
	bool CheckReleasedSetKey(); 

protected:
	// 이 스킬이 세팅된 키보드 키코드 
	TMap<FKey, bool> InputKeyTable;

	// 세팅한 키가 눌렸음 
	bool bCanInput = false; 


private:
	class APlayerController* PlayerController;
};
