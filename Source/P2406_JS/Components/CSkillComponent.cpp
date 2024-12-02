#include "Components/CSkillComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeaponData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Skill/CActiveSkill.h"

UCSkillComponent::UCSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


void UCSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (ActiveSkills.Num() > 0)
	{
		for (int i = 0; i < ActiveSkills.Num(); i++)
		{
			UCActiveSkill* active = ActiveSkills[i]; 
			active->Tick(DeltaTime);

			if (active->GetIsFinished())
				ActiveSkills.Remove(active);
		}
	}
}

void UCSkillComponent::ExecuteSkill(int32 InSlot)
{
	CheckTrue((ESkillSlot)InSlot >= ESkillSlot::Max);

	CLog::Print(FString::FromInt(InSlot)+ " Call Skill");

	if (SkillTable.Contains((ESkillSlot)InSlot) == true)
	{
		SkillTable[(ESkillSlot)InSlot]->ExecuteSkill();
		CurrentSkill = SkillTable[(ESkillSlot)InSlot];
		ActiveSkills.Add(SkillTable[(ESkillSlot)InSlot]);
	}
}

void UCSkillComponent::CreateSkillCollision()
{
	CheckNull(CurrentSkill);

	CurrentSkill->Create_Collision();
	
}

void UCSkillComponent::CreateSkillEffect()
{
	CheckNull(CurrentSkill);

	CurrentSkill->Create_Effect();
}

void UCSkillComponent::EndSkill()
{
	if (!!CurrentSkill)
	{
		CurrentSkill->EndSkill();
	}

	CurrentSkill = nullptr;
}

void UCSkillComponent::SetSkillList(const TArray<UCActiveSkill*>& InSkills)
{
	CheckFalse(InSkills.Num() > 0);

	int cnt = 0;
	for (UCActiveSkill* activeSkill : InSkills)
	{

		if ((ESkillSlot)cnt == ESkillSlot::Max)
			break;

		if (SkillTable.Contains((ESkillSlot)cnt) == true)
			SkillTable[(ESkillSlot)cnt] = activeSkill;
		else
			SkillTable.Emplace((ESkillSlot)cnt, activeSkill);
		cnt++;
	}

	CLog::Print("Skill Set Complete" + FString::FromInt(cnt));
}


