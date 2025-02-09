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

    // 1. OutJumpDoAction이 nullptr인지 확인
    CheckNull(OutJumpDoAction); 
    CheckNull(*OutJumpDoAction);

    // 2. UCDoAction_JumpAction*로 캐스팅
    UCDoAction_JumpAction* JumpAction = Cast<UCDoAction_JumpAction>(*OutJumpDoAction);
    CheckNull(JumpAction);

    // 3. JumpAction의 BeginPlay 호출
    JumpAction->InitializeFallAttackData(
        FallAttackActionDatas, 
        FallAttackHitDatas);
	

}

