#include "Skill/ActiveSkills/CActiveSkill_Instance.h"
#include "Global.h"

void UCActiveSkill_Instance::DefineSkillPhases()
{
	Super::DefineSkillPhases();
	
	SetupDefaultSkillPhase();
}