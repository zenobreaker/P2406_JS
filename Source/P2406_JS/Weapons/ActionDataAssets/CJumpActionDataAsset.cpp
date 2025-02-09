#include "Weapons/ActionDataAssets/CJumpActionDataAsset.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponData.h"

#include "Weapons/DoActions/CDoAction_JumpAction.h"


UCJumpActionDataAsset::UCJumpActionDataAsset()
{
}

void UCJumpActionDataAsset::BeginPlay(ACharacter* InOwner, UCWeaponData* InParentData, UCDoAction** OutJumpDoAction)
{
	if (InParentData == nullptr)
		return;
    
    Super::BeginPlay(InOwner, InParentData, OutJumpDoAction);

    // 1. OutJumpDoAction�� nullptr���� Ȯ��
    CheckNull(OutJumpDoAction); 
    CheckNull(*OutJumpDoAction);

    // 2. UCDoAction_JumpAction*�� ĳ����
    UCDoAction_JumpAction* JumpAction = Cast<UCDoAction_JumpAction>(*OutJumpDoAction);
    CheckNull(JumpAction);

    // 3. JumpAction�� BeginPlay ȣ��
    JumpAction->InitializeFallAttackData(
        FallAttackActionDatas, 
        FallAttackHitDatas);
	

}

