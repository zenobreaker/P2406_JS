#include "CSkillComponent.h"
#include "Skill/CSkillAsset.h"
#include "Components/CSkillComponent.h"
#include "Global.h"

UCSkillComponent::UCSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UCSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCSkillComponent::UseSkill(UCSkillAsset* InSkillAsset)
{
	CheckNull(InSkillAsset);

	
}

void UCSkillComponent::SetSkillList(const TArray<UCSkillAsset>& InSkills)
{
	CheckFalse(InSkills.Num() <= 0);

	for (UCSkillAsset skill : InSkills)
	{

	}
}

