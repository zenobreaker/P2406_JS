#pragma once

#include "CoreMinimal.h"
#include "Skill/ActiveSkills/CActvieSkill_Charge.h"
#include "Components/TimelineComponent.h"
#include "CActiveSkill_UpperSlash.generated.h"


/// <summary>
/// 이 스킬은 홀딩 기능이 내장되어 있다.
/// 짧게 누르면 빠르게 올려베고 어느 정도 충전하면 빠르게 전방으로 날아가 높이 벤다
/// </summary>
UCLASS()
class P2406_JS_API UCActiveSkill_UpperSlash
	: public UCActvieSkill_Charge

{
	GENERATED_BODY()

public:
	UCActiveSkill_UpperSlash();

public:
	void BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData) override; 
	void Tick(float InDeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	class UCurveFloat* Curve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Curve")
	float PlayRate = 100.0f; 

private:
	enum class UpperSlashState : uint8
	{
		UpperSlash = 0,
		GaleShash,
	};

protected:
	void DefineSkillPhases() override;

public:
	void ReleaseSkill() override;

protected:
	void Begin_Charging() override;
	void End_Charging() override;
	void Begin_Skill() override;
	void End_Skill() override;

public:
	void OffSkillDoAction() override;

public:
	UFUNCTION()
	void OnHoveringTarget(class ACharacter* InOther);

private:
	UFUNCTION()
	void OnRising(float Output);

private:
	UpperSlashState CurrentState;
	FVector Location;

private:
	FTimeline Timeline;
	FTimerHandle GravityResetHandle; 
	float OriginGravityScale = 0.0f; 
};