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

public:
	virtual void BeginPlay(
		ACharacter* InOwner,
		const TArray<FSkillActionData>& InDoActionDatas,
		const TArray<FSkillHitData>& InHitDatas) override;

	virtual void Tick(float InDeltaTime) override;

protected:
	// �Է� ���� Ű ���� 
	void SetSkillKey(EKeys InTargetKey); 

	// Ư�� Ű �Է� 
	virtual void Input_AnyKey(); 
	

	virtual void Begin_WaitInput() override;

private:
	void RegisterSkillPhase_Stance();

private:
	// �� ��ų�� ���õ� Ű���� Ű�ڵ� 
	EKeys SkillKey; 
	// ���� Ű 
	EKeys AttackKey;
};
