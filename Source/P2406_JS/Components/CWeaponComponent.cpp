#include "Components/CWeaponComponent.h"
#include "Global.h"
#include "CStateComponent.h"
#include "CSkillComponent.h"
#include "GameFramework/Character.h"
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
			DataAssets[i]->BeginPlay(OwnerCharacter, &Datas[i]);
		}
	}
	// 주인한테 있는 컴포넌트 가져옴
	Skill = CHelpers::GetComponent<UCSkillComponent>(OwnerCharacter);
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
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
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

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	class UCStateComponent* state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter, "State");
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

	if (!!Datas[(int32)InType])
	{
		Datas[(int32)InType]->GetEquipment()->Equip();

		if (!!Skill)
			Skill->SetSkillList(Datas[(int32)InType]->GetSkills());

		ChangeType(InType);
	}

	
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;

	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevType, InType);
}


void UCWeaponComponent::DoAction()
{
	if (!!GetDoAction())
	{
		GetDoAction()->DoAction();
	}
}

void UCWeaponComponent::ExecuteSkill(const int32 InIndex)
{
	CheckFalse(IsIdleMode());
	CheckNull(Skill);

	Skill->ExecuteSkill(InIndex);
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

