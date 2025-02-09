#include "Weapons/ActionDataAssets/CActionDataAsset.h"
#include "Global.h"

#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponData.h"

UCActionDataAsset::UCActionDataAsset()
{
}


void UCActionDataAsset::BeginPlay(ACharacter* InOwner, UCWeaponData* InParentData, UCDoAction** OutJumpDoAction)
{
	if (InParentData == nullptr)
		return; 

	ACAttachment* attachment = InParentData->GetAttachment();
	UCEquipment* equipemnt = InParentData->GetEquipment();

	UCDoAction* doAction = nullptr;
	if (!!DoActionClass)
	{
		doAction = NewObject<UCDoAction>(this, DoActionClass); 
		doAction->BeginPlay(InOwner, attachment, equipemnt, DoActionDatas, HitDatas);

		*OutJumpDoAction = doAction;
	}
	
}
