#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "Skill/Skill_Interface/InputableSkill.h"
#include "CActiveSkill_Stance.generated.h"

/// <summary>
///  �� ��ų�� Ư�� �Է��� �� �� ���� Ư�� ������ ���¿��� ��� �ϴ� Ư¡�� ������.
/// </summary>
UCLASS()
class P2406_JS_API UCActiveSkill_Stance 
	: public UCActiveSkill
	, public IInputableSkill
{
	GENERATED_BODY()


public:
	virtual void BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData) override;

protected:
	// �Է� ���� Ű ���� 
	void SetInputKey(FKey InTargetKey); 

	// Ư�� Ű �Է� 
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
	// �� ��ų�� ���õ� Ű���� Ű�ڵ� 
	TMap<FKey, bool> InputKeyTable;

	// ������ Ű�� ������ 
	bool bCanInput = false; 


private:
	class APlayerController* PlayerController;
};
