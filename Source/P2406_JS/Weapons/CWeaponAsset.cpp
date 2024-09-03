#include "Weapons/CWeaponAsset.h"
#include "Global.h"
#include "CAttachment.h"
#include "CEquipment.h"
//#include "CDoAction.h"
//#include "CSubAction.h"
#include "CWeaponData.h"
#include "GameFramework/Character.h"



UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter* InOwner, UCWeaponData** OutWeaponData)
{

	ACAttachment* attachment = nullptr; 
	if (!!AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		attachment = InOwner->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* equipment;
	if (!!EquipmentClass)
	{
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(InOwner, EquipmentData);

		if (!!attachment)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnequip.AddDynamic(attachment, &ACAttachment::OnUnequip);
		}

		*OutWeaponData = NewObject<UCWeaponData>();
		(*OutWeaponData)->Attachment = attachment;
		(*OutWeaponData)->Equipment = equipment;
		/*(*OutWeaponData)->DoAction = doAction;
		(*OutWeaponData)->SubAction = subAction;*/
	}
		
}
