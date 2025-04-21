﻿#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "Skill/Skill_Interface/ChargableSkill.h"
#include "CActvieSkill_Charge.generated.h"

/// 입력하는 시간에 따라 모션이 달리하는 스킬 
UCLASS()
class P2406_JS_API UCActvieSkill_Charge
	: public UCActiveSkill
	, public IChargableSkill
{
	GENERATED_BODY()


public:
	FORCEINLINE bool GetIsChargeTime() const { return ChargeTime > MinChargeTime; }
	FORCEINLINE float GetCurrentChargeTime() { return ChargeTime; }
	FORCEINLINE float GetMaxChargeTime() { return MaxChargeTime; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Info")
	float MaxChargeTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charge Info")
	float MinChargeTime = 0.15f; // 최소 충전 인정 시간 

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

	// IChargableSkill을(를) 통해 상속됨
	virtual void StartCharing() override;
	virtual void ReleaseCharge() override;

protected:
	bool bIsInput = false; 
	bool bIsExecuteInput = false;
	// 입력 받은 시간 
	float ChargeTime = 0.0f;
};
