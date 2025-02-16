#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "Skill/Skill_Interface/ChargableSkill.h"
#include "CActvieSkill_Charge.generated.h"

/// <summary>
/// �Է��ϴ� �ð��� ���� ����� �޸��ϴ� ��ų 
/// </summary>
UCLASS()
class P2406_JS_API UCActvieSkill_Charge
	: public UCActiveSkill
	, public IChargableSkill
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Info")
	float MaxChargeTime = 0.0f;

public:
	virtual void Tick(float InDeltaTime) override;

protected:
	ESkillPhase GetNextFlowPhase(ESkillPhase InPhase) override;
	virtual void Pressed() {};
	
public:
	virtual void ReleaseSkill() override; 

protected:
	virtual void Begin_Charging() {};
	virtual void End_Charging() {};

	// IChargableSkill��(��) ���� ��ӵ�
	virtual void StartCharing() override;
	virtual void ReleaseCharge() override;

protected:
	bool bIsInput = false; 
	bool bIsExecuteInput = false;
	// �Է� ���� �ð� 
	float ChargeTime = 0.0f;
};
