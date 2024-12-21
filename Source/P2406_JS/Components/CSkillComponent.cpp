#include "Components/CSkillComponent.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/CWeaponData.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Skill/CActiveSkill.h"
#include "GameInstances/CGameInstance.h"
#include "GameInstances/CSkillManager.h"

UCSkillComponent::UCSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	if (!!OwnerCharacter)
	{
		APlayerController* controller = OwnerCharacter->GetWorld()->GetFirstPlayerController();
		if (!!controller)
		{
			UCGameInstance* instance = Cast<UCGameInstance>(controller->GetGameInstance());
			if (instance == nullptr || instance->SkillManager == nullptr)
				return;

			SkillManager = instance->SkillManager;
		}
	}
}


void UCSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Update_CheckSkillComplete(DeltaTime);

	// SkillManager가 쿨다운 관리하므로 주석 
	//Update_SkillCooldown(DeltaTime);
}

void UCSkillComponent::ExecuteSkill(int32 InSlot)
{
	CheckTrue((ESkillSlot)InSlot >= ESkillSlot::Max);
	CheckTrue(SkillSlotTable.IsEmpty());
	CheckTrue(SkillSlotTable[(ESkillSlot)InSlot] == nullptr);

	CheckNull(SkillManager);

	//TODO: 사용하려는 스킬이 기존 다른 상태를 캔슬 할 수 있으면 이 로직은 수정


	// 스킬이 없으면 사용 불가능 
	if (SkillSlotTable.Contains((ESkillSlot)InSlot) == false)
		return;

	FLog::Print(FString::FromInt(InSlot) + " Call Skill");

	if (bIsSkillAction == true)
	{
		FLog::Print("Not Use a Skill Because Player is doing Skill");
		return;
	}

	// 해당 스킬의 상태를 체크하여 쿨다운이나 실행 여부 확인
	auto* skill = SkillSlotTable.FindRef((ESkillSlot)InSlot);
	if(skill ==nullptr)
	{
		FLog::Log("Skill is Nullptr");
		return; 
	}

	// 스킬 실행 
	bool check = false;
	//check |= skill->IsCooldown() == true;
	{
		int32 skillID = skill->GetSkillID();
		check |= SkillManager->IsCompleteCooldown(skillID);
	}

	if (check == false)
	{
		FLog::Print("Not Use a Skill Because Skill is Cooldown");

		return; // 무슨 버그인지는 몰라도 아래 check가 false여서 최종적으로 false인데도 불구하고 실행되는 버그
	}

	check |= skill->GetIsExecute() == false;

	if (check == true)
	{
		// 스킬 매니저에게 쿨다운 값 전달 
		{
			int32 skillID = skill->GetSkillID();
			float cooldown = skill->GetCooldown();
			SkillManager->ExecuteSkill(skillID, cooldown);
		}

		skill->ExecuteSkill();
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

		ActiveSkills.Add(skill);

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
	FLog::Print("Skill Comp => On Skill Cast");
	CurrentSkill->OnSkillCasting();

}
void UCSkillComponent::OffSkillCasting()
{
	CheckNull(CurrentSkill);
	FLog::Print("Skill Comp => Off Skill Cast");
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
void UCSkillComponent::SetEmptySkillList()
{
	SkillSlotTable.Empty();

	if (OnSkillSlotsCleared.IsBound())
		OnSkillSlotsCleared.Broadcast();
}

void UCSkillComponent::SetSkillList(const TArray<UCActiveSkill*>& InActiveSkills)
{
	//CheckFalse(InActiveSkills.Num() > 0);
	// 스킬이 아예 없으면 비워주고 처리함
	if(InActiveSkills.IsEmpty())
	{
		SetEmptySkillList(); 
		
		return; 
	}

	// 스킬 슬롯 한 번 정리 
	SetEmptySkillList();

	//TODO: 사용할 스킬 정보만 어케 따로 정리하는 내용이 필요하다 무기별로 사용가능 스킬은 여럿이 될 수 있으므로 
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

	FLog::Print("Skill Set Complete" + FString::FromInt(cnt));

	if (OnSetSkills.IsBound())
		OnSetSkills.Broadcast(InActiveSkills);
}

void UCSkillComponent::OnSkillSoaring()
{
	FLog::Log("Skill Soar Begin ");
	bIsSkillSoaring = true;
}

void UCSkillComponent::OffSkillSoaring()
{
	FLog::Log("Skill Soar End ");
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
				FLog::Print("Remove Skill " + active->GetName(), -1, 10.0f, FColor::Red);
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


