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
	// �������� �ִ� ������Ʈ ������
	SkillComp = FHelpers::GetComponent<UCSkillComponent>(OwnerCharacter);

	if (SkillComp != nullptr)
		REGISTER_EVENT_WITH_REPLACE(SkillComp, OnSkillExecuted, this, UCWeaponComponent::End_DoAction);

	// begin ó���� �� �Ǹ� ȣ��
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


	// �ش� ������ ��ų ����
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
	// ��ų�� ���� Ÿ���̳� �׷��Ÿ� �ƹ��͵� ���� ���� 
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
		// ���Ⱑ ���� ȣ�� �ǰ� �ִ��ν��Ͻ����� �ٽ� 
		// ���� ��Ÿ�� ����ؼ� ���İ����� �̻��ϰ� ��µǳ�
		// docation���� �� ������Ʈ ���� ��Ȯ�� ó���ϰ� �ؾ���

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
	//TODO: ��ų�� �Ϻ� ��� ĵ�� �����̶�� �� ������ �����ؾ�������. 
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

	// ��� ���� ����� ����׼ǹ��⿡�� ����
	if (!!GetSubAction())
		return GetSubAction()->TryGuard(DamageData);

	return false;
}

