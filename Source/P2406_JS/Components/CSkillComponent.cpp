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

void UCSkillComponent::ExecuteSkill(int32 InSlot)
{
	CheckTrue((ESkillSlot)InSlot >= ESkillSlot::Max);

	CLog::Print(FString::FromInt(InSlot)+ " Call Skill");
	//TODO: Skill Action »£√‚
	//SkillTable[InSlot];
}

void UCSkillComponent::SetSkillList(const TArray<class UCSkillAsset>& InSkills)
{
	CheckFalse(InSkills.Num() <= 0);

	int cnt = 0;
	for (const UCSkillAsset& skillAsset : InSkills)
	{
		
		if ((ESkillSlot)cnt == ESkillSlot::Max)
			break; 
		//SkillTable.Emplace((ESkillSlot)cnt, skillAsset);
		//cnt++; 
	}

}

