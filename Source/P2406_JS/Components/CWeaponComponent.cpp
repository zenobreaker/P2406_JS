#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "CSkillComponent.h"
#include "GameFramework/Character.h"
//#include "GameFramework/CharacterMovementComponent.h"

#include <Characters/CPlayer.h>
#include "Weapons/CWeaponAsset.h"
#include "Weapons/CWeaponData.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"
#include "Skill/CActiveSkill.h"


UCWeaponComponent::UCWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (!!DataAssets[i])
		{
			DataAssets[i]->WA_BeginPlay(OwnerCharacter, &Datas[i]);
		}
	}
	// 주인한테 있는 컴포넌트 가져옴
	SkillComp = FHelpers::GetComponent<UCSkillComponent>(OwnerCharacter);

	if (SkillComp != nullptr)
		REGISTER_EVENT_WITH_REPLACE(SkillComp, OnSkillExecuted, this, UCWeaponComponent::End_DoAction);

	// begin 처리가 다 되면 호출
	InitEquip();
}


void UCWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);

	if (!!GetSubAction())
		GetSubAction()->Tick(DeltaTime);
}

bool UCWeaponComponent::IsIdleMode()
{
	return FHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetAttachment();
}

UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetEquipment();
}

UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetDoAction();
}

UCDoAction* UCWeaponComponent::GetJumpDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetJumpAction();
}

UCSubAction* UCWeaponComponent::GetSubAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!Datas[(int32)Type], nullptr);

	return Datas[(int32)Type]->GetSubAction();
}

UCWeaponData* UCWeaponComponent::GetCurrentWeaponData()
{
	return Datas[(int32)Type];
}


void UCWeaponComponent::SetUnarmedMode()
{
	CheckNull(GetEquipment());

	GetEquipment()->Unequip();

	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetFistMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Fist);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetHammerMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Hammer);
}

void UCWeaponComponent::SetWarpMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Warp);
}


void UCWeaponComponent::SetBowMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Bow);
}

void UCWeaponComponent::InitEquip()
{
	SetMode(InitType);
}


void UCWeaponComponent::SetMode(EWeaponType InType)
{
	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter, "State");

	if (state != nullptr)
	{
		if (state->IsActionMode())
			return;
	}

	if (Type == InType)
	{
		SetUnarmedMode();

		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->Unequip();
	}


	// 해당 무기의 스킬 세팅
	if (!!Datas[(int32)InType])
	{
		Datas[(int32)InType]->GetEquipment()->Equip();

		UCSubAction* subaction = Datas[(int32)InType]->GetSubAction();

		REGISTER_EVENT_WITH_REPLACE(subaction, OnGuardValueChanged, this, UCWeaponComponent::ChangeGuardValue);
	}

	ChangeType(InType);
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;


	if (InType != EWeaponType::Max && !!SkillComp)
		SkillComp->SetSkillList(Datas[(int32)InType]->GetSkills());
	// 스킬이 없는 타입이나 그런거면 아무것도 없이 보내 
	else if (InType == EWeaponType::Max)
		SkillComp->SetEmptySkillList();


	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevType, InType);
}

void UCWeaponComponent::PlayFallingAttackMontage()
{
	CheckNull(GetJumpDoAction());

	GetJumpDoAction()->PlayFallAttackMontage();
}

void UCWeaponComponent::ChangeGuardValue(float InValue, float InMaxValue)
{
	if (OnGuardValueChanged.IsBound())
		OnGuardValueChanged.Broadcast(InValue, InMaxValue);
}


void UCWeaponComponent::DoAction()
{
	bool canInput = true;
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != nullptr)
		canInput = player->GetCanInput();

	CheckFalse(canInput);

	Handle_DoAction();
}

void UCWeaponComponent::DoAction_Heavy()
{
	bool canInput = true;
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != nullptr)
		canInput = player->GetCanInput();

	CheckFalse(canInput);

	Handle_DoAction(true);
}

void UCWeaponComponent::Begin_DoAction()
{
	//if (!!GetDoAction())
	//{
	//	GetDoAction()->Begin_DoAction();
	//}
	Handle_BeginDoAction();

	DYNAMIC_EVENT_CALL(OnBeginDoAction);
}

void UCWeaponComponent::End_DoAction()
{
	/*if (!!GetDoAction())
	{
		GetDoAction()->End_DoAction();
	}*/
	Handle_EndDoAction();

	DYNAMIC_EVENT_CALL(OnEndedDoAction);
}

/// <summary>
/// Handle Func
/// </summary>
//--------------------------------------------------------------------------------
void UCWeaponComponent::Handle_DoAction(bool InHeavyValue)
{
	CheckNull(GetDoAction());

	// Jump
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != nullptr && player->IsJumping() == true)
	{
		CheckNull(GetJumpDoAction());
		// 여기가 먼저 호출 되고 애님인스턴스에서 다시 
		// 폴링 몽타주 재생해서 겹쳐가지고 이상하게 출력되네
		// docation에서 각 스테이트 별로 정확히 처리하게 해야해

		GetJumpDoAction()->SetHeavyActionFlag(InHeavyValue);

		GetJumpDoAction()->DoAction();

		return;
	}

	// Plane 
	GetDoAction()->DoAction();

	return;
}

void UCWeaponComponent::Handle_BeginDoAction()
{
	CheckNull(GetDoAction());

	// Jump
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != nullptr)
	{
		if (player->IsJumping() == true || bIsFallingAttack == true)
		{
			CheckNull(GetJumpDoAction());

			GetJumpDoAction()->Begin_DoAction();

			return;
		}
	}

	// Plane 
	GetDoAction()->Begin_DoAction();

	return;
}

void UCWeaponComponent::Handle_EndDoAction()
{
	CheckNull(GetDoAction());
	// Jump
	ACPlayer* player = Cast<ACPlayer>(OwnerCharacter);
	if (player != nullptr)
	{
		if (player->IsJumping() == true || bIsFallingAttack == true)
		{
			CheckNull(GetJumpDoAction());
			
			GetJumpDoAction()->End_DoAction();

			return;
		}
	}

	// Plane 
	GetDoAction()->End_DoAction();
}

/// <summary>
/// 
/// </summary>
//----------------------------------------------------------------------------------------
void UCWeaponComponent::ExecuteSkill(const int32 InIndex)
{
	FLog::Log("Call Exectue Skill");
	//TODO: 스킬이 일부 모션 캔슬 가능이라면 이 로직은 수정해야할지도. 
	//CheckFalse(IsIdleMode());
	CheckNull(SkillComp);

	SkillComp->ExecuteSkill(InIndex);
}

void UCWeaponComponent::ReleaseSkill(const int32 InIndex)
{
	CheckNull(SkillComp);

	SkillComp->ReleaseSkill(InIndex);
}

void UCWeaponComponent::SubAction_Pressed()
{
	if (!!GetSubAction())
		GetSubAction()->Pressed();
}

void UCWeaponComponent::SubAction_Released()
{
	if (!!GetSubAction())
		GetSubAction()->Released();
}

bool UCWeaponComponent::TryGuard(ACBaseCharacter::FDamageData& DamageData)
{
	CheckTrueResult(OwnerCharacter == nullptr, false);

	UCDoAction* doAction = GetDoAction();
	if (!!doAction)
	{
		doAction->End_DoAction();
	}

	// 방어 각도 계산은 서브액션무기에게 전달
	if (!!GetSubAction())
		return GetSubAction()->TryGuard(DamageData);

	return false;
}

