#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "CActiveSkill_Stance.generated.h"

/// <summary>
///  �� ��ų�� Ư�� �Է��� �� �� ���� Ư�� ������ ���¿��� ��� �ϴ� Ư¡�� ������.
/// </summary>
UCLASS()
class P2406_JS_API UCActiveSkill_Stance : public UCActiveSkill
{
	GENERATED_BODY()


protected:
	// �Է� ���� Ű ���� 
	void SetSkillKey(EKeys InTargetKey); 

	// Ư�� Ű �Է� 
	virtual void Input_AnyKey(); 
	

	virtual void Begin_WaitInput() override;

protected:
	virtual void DefineSkillPhases() override;

private:
	// �� ��ų�� ���õ� Ű���� Ű�ڵ� 
	EKeys SkillKey; 
	// ���� Ű 
	EKeys AttackKey;
};
