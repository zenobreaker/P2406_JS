#include "Weapons/SubActions/CSubAction_Sword_Guard.h"
#include "Weapons/CSubAction.h"
#include "Global.h"
#include "GameFramework/Character.h"

UCSubAction_Sword_Guard::UCSubAction_Sword_Guard()
{

}

void UCSubAction_Sword_Guard::Pressed()
{
	CheckNull(Owner);

	Super::Pressed();

	Owner->PlayAnimMontage(GuardMontage);

	Owner->GetWorld()->GetTimerManager().SetTimer(GuardTimer, this, &UCSubAction_Sword_Guard::Evaluate_GuradStance, 0.75f, false);
}

void UCSubAction_Sword_Guard::OnPressSpecialAction()
{

}

void UCSubAction_Sword_Guard::Begin_DoSubAction_Implementation()
{
}

void UCSubAction_Sword_Guard::End_DoSubAction_Implementation()
{
}

void UCSubAction_Sword_Guard::Evaluate_GuradStance()
{
}
