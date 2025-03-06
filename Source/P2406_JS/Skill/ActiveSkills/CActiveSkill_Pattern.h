#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "CActiveSkill_Pattern.generated.h"

// AI 패턴용 스킬 
UCLASS()
class P2406_JS_API UCActiveSkill_Pattern : public UCActiveSkill
{
	GENERATED_BODY()
	
protected:
	virtual void DefineSkillPhases() override;

public:
	virtual void Begin_Skill() override;
	virtual void Create_SkillEffect() override; 
};
