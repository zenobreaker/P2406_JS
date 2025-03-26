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

	// SkillManager가 쿨다운 관리하므로 주석 
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

	// 해당 스킬의 상태를 체크하여 쿨다운이나 실행 여부 확인
	UCActiveSkill* skill = SkillSlotTable.FindRef((ESkillSlot)InSlot);
	CheckNull(skill);
	CheckTrue(skill->GetIsExecute());

	// 스킬 실행 
	bool check = false;
	{
		int32 skillID = skill->GetSkillID();
		check |= SkillManager->IsCompleteCooldown(skillID);
	}
	// 무슨 버그인지는 몰라도 아래 check가 false여서 최종적으로 false인데도 불구하고 실행되는 버그
	CheckFalse(check);

	//TODO: 사용하려는 스킬이 기존 다른 상태를 캔슬 할 수 있으면 이 로직은 수정
	DYNAMIC_EVENT_CALL(OnSkillExecuted);

	// 스킬 매니저에게 쿨다운 값 전달 
	{
		int32 skillID = skill->GetSkillID();
		float cooldown = skill->GetCooldown();
		SkillManager->ExecuteSkill(skillID, cooldown);
	}

	// 해당 스킬의 종료 콜백 설정 
	REGISTER_EVENT_WITH_REPLACE(skill, OnSkillEnded, this, UCSkillComponent::EndedSkill);
	
	// 스킬 실행 
	skill->ExecuteSkill();

	// 실행된 스킬 목록에 넣어놓기
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


//TODO : Input System 을 개편하면 이 내용을 수정 
void UCSkillComponent::HandleChargingSkill(/*UCActiveSkill* InActiveSkill*/)
{
	//CheckNull(InActiveSkill);
	CheckNull(CurrentSkill);

	// 이 스킬이 충전 스킬인지 검사 
	UCActvieSkill_Charge* charge = Cast<UCActvieSkill_Charge>(CurrentSkill);
	CheckNull(charge); 

	HandleChargingSkill_Visible(charge);

	HandleChargingSkill_Updated(charge);
}

void UCSkillComponent::HandleChargingSkill_Visible(UCActvieSkill_Charge* InChargeSkill)
{
	//CheckNull(InActiveSkill);
	CheckNull(InChargeSkill);

	// 현재 스킬이 충전 중인지 확인한다. 
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnUpdatedChargeVisiable, InChargeSkill->GetIsChargeTime());

}

void UCSkillComponent::HandleChargingSkill_Updated(UCActvieSkill_Charge* InChargeSkill)
{
	//CheckNull(InActiveSkill);
	CheckNull(InChargeSkill);

	// 충전 정보
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
	// 스킬이 아예 없으면 비워주고 처리함
	if (InActiveSkills.IsEmpty())
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


// TODO: 만약 다음에 또 같은 기능을 구현한다면 아래 기능은 뺀다..
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
