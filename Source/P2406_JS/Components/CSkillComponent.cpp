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
	
	Update_CheckSkillComplete(DeltaTime);

	Update_SkillCooldown(DeltaTime);
}

void UCSkillComponent::ExecuteSkill(int32 InSlot)
{
	CheckTrue((ESkillSlot)InSlot >= ESkillSlot::Max);

	//TODO: 사용하려는 스킬이 기존 다른 상태를 캔슬 할 수 있으면 이 로직은 수정
	


	// 스킬이 없으면 사용 불가능 
	if (SkillSlotTable.Contains((ESkillSlot)InSlot) == false)
		return;
	
	CLog::Print(FString::FromInt(InSlot)+ " Call Skill");

	if(bIsSkillAction == true)
	{
		CLog::Print("Not Use a Skill Because Player is doing Skill");
		return; 
	}

	// 스킬 실행 
	bool check = false; 
	check |= SkillSlotTable[(ESkillSlot)InSlot]->IsCoolDown() == true;
	if (check == false)
	{
		CLog::Print("Not Use a Skill Because Skill is Cooldown");
	}
	check |= SkillSlotTable[(ESkillSlot)InSlot]->GetIsExecute() == false; 
	
	if(check)
	{
		SkillSlotTable[(ESkillSlot)InSlot]->ExecuteSkill();
		CurrentSkill = SkillSlotTable[(ESkillSlot)InSlot];
		if (CurrentSkill != nullptr)
		{
			CurrentSkill->OnActionBegin.Clear(); 
			CurrentSkill->OnActionEnd.Clear(); 

			CurrentSkill->OnSoaringBegin.Clear();
			CurrentSkill->OnSoaringEnd.Clear();

			CurrentSkill->OnSoaringBegin.AddDynamic(this, &UCSkillComponent::OnSkillSoaring);
			CurrentSkill->OnSoaringEnd.AddDynamic(this, &UCSkillComponent::OffSkillSoaring);
		}

		ActiveSkills.Add(SkillSlotTable[(ESkillSlot)InSlot]);
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


//////////////////////////////////////////////////////////////////////////////////////////
void UCSkillComponent::BeginSkill()
{
	bIsSkillAction = true; 
}


void UCSkillComponent::EndSkill()
{
	if (!!CurrentSkill)
	{
		CurrentSkill->EndSkill();
	}

	bIsSkillAction = false; 
	CurrentSkill = nullptr;
}
void UCSkillComponent::OnSkillCasting()
{
	CheckNull(CurrentSkill);
	CLog::Print("Skill Comp => On Skill Cast");
	CurrentSkill->OnSkillCasting(); 

}
void UCSkillComponent::OffSkillCasting()
{
	CheckNull(CurrentSkill);
	CLog::Print("Skill Comp => Off Skill Cast");
	CurrentSkill->OffSkillCasting();
}

void UCSkillComponent::OnSkillDoAction()
{
	CheckNull(CurrentSkill);
	CurrentSkill->OnSkillDoAction(); 
}

void UCSkillComponent::OffSkillDoAction()
{
	CheckNull(CurrentSkill);
	CurrentSkill->OffSkillDoAction();
}

//////////////////////////////////////////////////////////////////////////////////////////
void UCSkillComponent::SetSkillList(const TArray<UCActiveSkill*>& InActiveSkills)
{
	CheckFalse(InActiveSkills.Num() > 0);

	int cnt = 0;
	for (UCActiveSkill* activeSkill : InActiveSkills)
	{

		if ((ESkillSlot)cnt == ESkillSlot::Max)
			break;

		if (SkillSlotTable.Contains((ESkillSlot)cnt) == true)
			SkillSlotTable[(ESkillSlot)cnt] = activeSkill;
		else
			SkillSlotTable.Emplace((ESkillSlot)cnt, activeSkill);
		cnt++;
	}

	CLog::Print("Skill Set Complete" + FString::FromInt(cnt));

	if (OnSetSkills.IsBound())
		OnSetSkills.Broadcast(InActiveSkills);
}

void UCSkillComponent::OnSkillSoaring()
{
	CLog::Log("Skill Soar Begin ");
	bIsSkillSoaring = true;
}

void UCSkillComponent::OffSkillSoaring()
{
	CLog::Log("Skill Soar End ");
	bIsSkillSoaring = false; 
}

void UCSkillComponent::Update_CheckSkillComplete(float InDeltaTime)
{
	if (ActiveSkills.Num() > 0)
	{
		for (int i = 0; i < ActiveSkills.Num(); i++)
		{
			UCActiveSkill* active = ActiveSkills[i];
			active->Tick(InDeltaTime);

			if (active->GetIsFinished())
			{
				CLog::Print("Remove Skill " + active->GetName(), -1 , 10.0f, FColor::Red);
				ActiveSkills.Remove(active);
			}
		}
	}
}

void UCSkillComponent::Update_SkillCooldown(float InDeltaTime)
{
	for (auto& skillSlot : SkillSlotTable)
	{
		if (skillSlot.Value == nullptr)
			continue; 

		skillSlot.Value->Update_Cooldown(InDeltaTime);
	}
}


