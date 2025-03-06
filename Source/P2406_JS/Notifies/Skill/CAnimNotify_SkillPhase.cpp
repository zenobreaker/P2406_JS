#include "Notifies/Skill/CAnimNotify_SkillPhase.h"
#include "Global.h"

#include "Components/CSkillComponent.h"
#include "Components/CPatternComponent.h"
#include "Skill/CActiveSkill.h"

UCAnimNotify_SkillPhase::UCAnimNotify_SkillPhase()
{
#if WITH_EDITORONLY_DATA
	FLinearColor color = FLinearColor(0.3f, 0.5f, 0.5f);
	NotifyColor = color.ToFColor(true);
#endif 
}

FString UCAnimNotify_SkillPhase::GetNotifyName_Implementation() const
{
	return GetNotifyName("SkillPhase", Type);
}

void UCAnimNotify_SkillPhase::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCSkillComponent* skill = FHelpers::GetComponent<UCSkillComponent>(MeshComp->GetOwner());
	UCActiveSkill* activeSkill = nullptr;
	if (skill == nullptr)
	{
		UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(MeshComp->GetOwner());
		if (pattern != nullptr)
		{
			activeSkill = pattern->GetCurrentActiveSkill();
		}
	}
	else 
		activeSkill = skill->GetCurrentActiveSkill();
	CheckNull(activeSkill); 

	switch (Type)
	{
	case ESkillPhase::Start:
		break;
	case ESkillPhase::Begin_Casting:
		activeSkill->Begin_Casting();
		break;
	case ESkillPhase::End_Casting:
		activeSkill->End_Casting();
		break;
	case ESkillPhase::Begin_Skill:
		activeSkill->Begin_Skill();
		break;
	case ESkillPhase::End_Skill:
		activeSkill->End_Skill();
		break;
	case ESkillPhase::Finished:
		activeSkill->Finish_Skill();
		break;
	case ESkillPhase::Create_Effect:
		activeSkill->Create_SkillEffect();
		break; 
	case ESkillPhase::Max:
		break;
	}
}

FString UCAnimNotify_SkillPhase::GetNotifyName(FString InName, ESkillPhase InType) const
{
	if (InType != ESkillPhase::Max)
		return InName + "_" + StaticEnum<ESkillPhase>()->GetNameStringByValue((int64)InType);

	return InName;
}
