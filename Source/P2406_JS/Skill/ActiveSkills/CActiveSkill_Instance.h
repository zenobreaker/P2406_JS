#pragma once

#include "CoreMinimal.h"
#include "Skill/CActiveSkill.h"
#include "CActiveSkill_Instance.generated.h"

/// <summary>
/// 일반적으로 스킬 기능을 수행하는 클래스 
/// </summary>
UCLASS()
class P2406_JS_API UCActiveSkill_Instance : public UCActiveSkill
{
	GENERATED_BODY()
	

protected:
	virtual void DefineSkillPhases() override; 

public:
	virtual void Begin_Casting() override;
	virtual void Begin_Skill() override;
};
