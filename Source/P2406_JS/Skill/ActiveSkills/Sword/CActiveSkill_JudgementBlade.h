#pragma once

#include "CoreMinimal.h"
#include "Skill/ActiveSkills/CActiveSkill_Instance.h"
#include "CActiveSkill_JudgementBlade.generated.h"


/// <summary>
/// 기를 모은 후에 검을 휘둘러 주변 공간을 베는 기술  
/// </summary>
UCLASS()
class P2406_JS_API UCActiveSkill_JudgementBlade : public UCActiveSkill_Instance
{
	GENERATED_BODY()
	

protected:
	void DefineSkillPhases() override;

public:
	void Begin_Casting() override;
	void Begin_Skill() override;

private: 
	UFUNCTION()
	void OnSkillCastingCompleted_JudgementBlade();

	UFUNCTION()
	void OnSkillEnd_JudgementBlade();
};
