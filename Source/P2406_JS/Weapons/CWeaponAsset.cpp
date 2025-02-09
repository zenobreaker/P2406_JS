#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"
#include "CSubAction.h"
#include "CWeaponData.h"
#include "GameFramework/Character.h"
#include "SubActions/CSubAction_Sword.h"
#include <Skill/CActiveSkill.h>





UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::WA_BeginPlay(ACharacter* InOwner, UCWeaponData** OutWeaponData)
{

	ACAttachment* attachment = nullptr;
	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);

		ACBaseCharacter* baseCharacter = Cast<ACBaseCharacter>(InOwner);
		if (baseCharacter != nullptr)
		{
			REGISTER_EVENT_WITH_REPLACE(baseCharacter, OnCharacterDead, attachment, ACAttachment::OnDestroy);
		}
	}

	UCEquipment* equipment = nullptr;
	if (!!EquipmentClass)
	{
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, EquipmentData);

		if (!!attachment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}
	}

	UCDoAction* doAction = nullptr;
	if (!!DoActionClass)
	{
		doAction = NewObject<UCDoAction>(this, DoActionClass);
		doAction->BeginPlay(InOwner, attachment, equipment, DoActionDatas, HitDatas);

		if (!!attachment)
		{
			attachment->OnAttachmentBeginCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginCollision);
			attachment->OnAttachmentEndCollision.AddDynamic(doAction, &UCDoAction::OnAttachmentEndCollision);

			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}

		if (!!equipment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(doAction, &UCDoAction::OnUnequip);
		}
	}

	UCSubAction* subAction = nullptr;
	if (!!SubActionClass)
	{
		subAction = NewObject<UCSubAction>(this, SubActionClass);
		subAction->BeginPlay(InOwner, attachment, doAction);
	}

	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->Attachment = attachment;
	(*OutWeaponData)->Equipment = equipment;
	(*OutWeaponData)->DoAction = doAction;
	(*OutWeaponData)->SubAction = subAction;

	// WeaponAsset에 있는 스킬 정보를 가져옴 
	if (ActiveSkillAssets.Num() > 0)
	{
		for (int32 i = 0; i < ActiveSkillAssets.Num(); i++)
		{
			UCActiveSkill* activeSkill = nullptr;
			ActiveSkillAssets[i]->BeginPlay(InOwner, &activeSkill);

			if (!!activeSkill)
				(*OutWeaponData)->Skills.Add(activeSkill);
		}
	}
	
	UCDoAction* jumpDoAction = nullptr;
	if (!!JumpAcitonAsset)
	{
		JumpAcitonAsset->BeginPlay(InOwner, *OutWeaponData, &jumpDoAction);
		(*OutWeaponData)->JumpAction = jumpDoAction;
	}
}
