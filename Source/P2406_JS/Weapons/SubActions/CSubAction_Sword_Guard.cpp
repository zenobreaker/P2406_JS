#include "Weapons/SubActions/CSubAction_Sword_Guard.h"
#include "Global.h"

#include "Characters/CBaseCharacter.h"
#include "Characters/CPlayer.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

#include "Components/CGuardComponent.h"

#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"
#include "Weapons/DoActions/CDoAction_Combo.h"

UCSubAction_Sword_Guard::UCSubAction_Sword_Guard()
{

}

void UCSubAction_Sword_Guard::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	Guard = FHelpers::GetComponent<UCGuardComponent>(InOwner);
}

void UCSubAction_Sword_Guard::OffGuardStance()
{
	if (!!Guard)
		Guard->StopGuard();
}

void UCSubAction_Sword_Guard::Pressed()
{
	Super::Pressed();

	if (!!Guard)
		Guard->StartGuard();
}

void UCSubAction_Sword_Guard::Released()
{
	OffGuardStance();
}


// 이 함수가 호출되었다는 건 데미지가 들어왔다는 것 
bool UCSubAction_Sword_Guard::TryGuard(ACharacter* InAttacker)
{
	CheckNullResult(Guard, Super::TryGuard(InAttacker));

	return Guard->CheckBlocking(InAttacker);
}


void UCSubAction_Sword_Guard::End_CounterAttack()
{
	ACPlayer* player = Cast<ACPlayer>(Owner);
	if (!!player)
		*player->bCountering = false;
}


