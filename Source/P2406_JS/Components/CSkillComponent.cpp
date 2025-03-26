#include "Components/CSkillComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CWeaponComponent.h"
#include "Components/CMovementComponent.h"
#include "GameInstances/CGameInstance.h"
#include "GameInstances/CSkillManager.h"

#include "Skill/CActiveSkill.h"
#include "Skill/ActiveSkills/CActvieSkill_Charge.h"
#include "Weapons/CWeaponData.h"


UCSkillComponent::UCSkillComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCSkillComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	APlayerController* controller = OwnerCharacter->GetWorld()->GetFirstPlayerController();
	if (!!controller)
	{
		UCGameInstance* instance = Cast<UCGameInstance>(controller->GetGameInstance());
		if (instance == nullptr || instance->SkillManager == nullptr)
			return;

		SkillManager = instance->SkillManager;
	}

	DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedChargeVisiable, false);

	Movement = FHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	if (Movement != nullptr)
		bCanMove = Movement->GetCanMovePtr();
}


void UCSkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Update_CheckSkillComplete(DeltaTime);
	
	HandleChargingSkill();

	// SkillManager�� ��ٿ� �����ϹǷ� �ּ� 
	//Update_SkillCooldown(DeltaTime);
}

void UCSkillComponent::ExecuteSkill(int32 InSlot)
{
	CheckNull(SkillManager);
	CheckTrue((ESkillSlot)InSlot >= ESkillSlot::Max);
	CheckTrue(SkillSlotTable.IsEmpty());
	CheckTrue(SkillSlotTable[(ESkillSlot)InSlot] == nullptr);
	CheckTrue(bIsSkillAction);
	CheckFalse(SkillSlotTable.Contains((ESkillSlot)InSlot));

	// �ش� ��ų�� ���¸� üũ�Ͽ� ��ٿ��̳� ���� ���� Ȯ��
	UCActiveSkill* skill = SkillSlotTable.FindRef((ESkillSlot)InSlot);
	CheckNull(skill);
	CheckTrue(skill->GetIsExecute());

	// ��ų ���� 
	bool check = false;
	{
		int32 skillID = skill->GetSkillID();
		check |= SkillManager->IsCompleteCooldown(skillID);
	}
	// ���� ���������� ���� �Ʒ� check�� false���� ���������� false�ε��� �ұ��ϰ� ����Ǵ� ����
	CheckFalse(check);

	//TODO: ����Ϸ��� ��ų�� ���� �ٸ� ���¸� ĵ�� �� �� ������ �� ������ ����
	DYNAMIC_EVENT_CALL(OnSkillExecuted);

	// ��ų �Ŵ������� ��ٿ� �� ���� 
	{
		int32 skillID = skill->GetSkillID();
		float cooldown = skill->GetCooldown();
		SkillManager->ExecuteSkill(skillID, cooldown);
	}

	// �ش� ��ų�� ���� �ݹ� ���� 
	REGISTER_EVENT_WITH_REPLACE(skill, OnSkillEnded, this, UCSkillComponent::EndedSkill);
	
	// ��ų ���� 
	skill->ExecuteSkill();

	// ����� ��ų ��Ͽ� �־����
	ActiveSkills.Add(skill);

	CurrentSkill = skill;
}

void UCSkillComponent::ReleaseSkill(int32 InSlot)
{
	CheckFalse(SkillSlotTable.Contains((ESkillSlot)InSlot));

	SkillSlotTable[(ESkillSlot)InSlot]->ReleaseSkill();
}


//////////////////////////////////////////////////////////////////////////////////////////
void UCSkillComponent::BeginSkill()
{
	bIsSkillAction = true;
}

void UCSkillComponent::EndSkill()
{
	bIsSkillAction = false;
	DYNAMIC_EVENT_CALL(OnCurrentSkillEnded);
	
	if (!!CurrentSkill)
		CurrentSkill->CompleteSkill();
	CurrentSkill = nullptr;
	
	if (bCanMove != nullptr && *bCanMove == false)
		Movement->Move();
}


//TODO : Input System �� �����ϸ� �� ������ ���� 
void UCSkillComponent::HandleChargingSkill(/*UCActiveSkill* InActiveSkill*/)
{
	//CheckNull(InActiveSkill);
	CheckNull(CurrentSkill);

	// �� ��ų�� ���� ��ų���� �˻� 
	UCActvieSkill_Charge* charge = Cast<UCActvieSkill_Charge>(CurrentSkill);
	CheckNull(charge); 

	HandleChargingSkill_Visible(charge);

	HandleChargingSkill_Updated(charge);
}

void UCSkillComponent::HandleChargingSkill_Visible(UCActvieSkill_Charge* InChargeSkill)
{
	//CheckNull(InActiveSkill);
	CheckNull(InChargeSkill);

	// ���� ��ų�� ���� ������ Ȯ���Ѵ�. 
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedChargeVisiable, InChargeSkill->GetIsChargeTime());

}

void UCSkillComponent::HandleChargingSkill_Updated(UCActvieSkill_Charge* InChargeSkill)
{
	//CheckNull(InActiveSkill);
	CheckNull(InChargeSkill);

	// ���� ����
	DYNAMIC_EVENT_CALL_TWO_PARAMS(OnUpdatedChargeGauge,
		InChargeSkill->GetCurrentChargeTime(),
		InChargeSkill->GetMaxChargeTime());
}

//-----------------------------------------------------------------------------
void UCSkillComponent::EndedSkill()
{
	EndSkill();
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
	// ��ų�� �ƿ� ������ ����ְ� ó����
	if (InActiveSkills.IsEmpty())
	{
		SetEmptySkillList();

		return;
	}

	// ��ų ���� �� �� ���� 
	SetEmptySkillList();

	//TODO: ����� ��ų ������ ���� ���� �����ϴ� ������ �ʿ��ϴ� ���⺰�� ��밡�� ��ų�� ������ �� �� �����Ƿ� 
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

	//FLog::Print("Skill Set Complete" + FString::FromInt(cnt));

	if (OnSetSkills.IsBound())
		OnSetSkills.Broadcast(InActiveSkills);
}


void UCSkillComponent::Update_CheckSkillComplete(float InDeltaTime)
{
	if (ActiveSkills.Num() > 0)
	{
		for (int i = ActiveSkills.Num() - 1; i >= 0; i--)
		{
			UCActiveSkill* active = ActiveSkills[i];
			active->Tick(InDeltaTime);

			if (active->GetIsFinished())
			{
				FLog::Log("Remove Skill " + active->GetName());
				ActiveSkills.RemoveAt(i);
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

void UCSkillComponent::OnActivated_Collision(FName InName)
{
	CheckNull(CurrentSkill);

	CurrentSkill->OnActivated_Collision(InName);
}

void UCSkillComponent::OnDeactivated_Collision(FName InName)
{
	CheckNull(CurrentSkill);

	CurrentSkill->OnDeactivated_Collision(InName);
}


// TODO: ���� ������ �� ���� ����� �����Ѵٸ� �Ʒ� ����� ����..
//----------------------------------------------------------------------------------------
void UCSkillComponent::OnSkillCasting()
{
	CheckNull(CurrentSkill);
	CurrentSkill->OnSkillCasting();

}
void UCSkillComponent::OffSkillCasting()
{
	CheckNull(CurrentSkill);
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
