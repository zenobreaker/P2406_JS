#include "Notifies/CAnimNotifyState_Combo.h"
#include "Global.h"

#include "Characters/CBaseCharacter.h"
#include "Components/CWeaponComponent.h"
#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Weapons/IComboable.h"

FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());

	/*UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	CheckNull(combo);

	combo->EnableCombo();*/

	ACBaseCharacter* base = Cast<ACBaseCharacter>(MeshComp->GetOwner());
	CheckNull(base); 

	if (base->IsJumping() == true)
	{
		IIComboable* combo = Cast<IIComboable>(weapon->GetJumpDoAction());
		CheckNull(combo);
		combo->EnableCombo();
	}
	else
	{
		IIComboable* combo = Cast<IIComboable>(weapon->GetDoAction());
		CheckNull(combo);
		combo->EnableCombo();
	}
	
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	Super::NotifyEnd(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCWeaponComponent* weapon = FHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetDoAction());


	/*UCDoAction_Combo* combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	CheckNull(combo);

	combo->DisableCombo();*/

	ACBaseCharacter* base = Cast<ACBaseCharacter>(MeshComp->GetOwner());
	CheckNull(base);

	if (base->IsJumping() == true)
	{
		IIComboable* combo = Cast<IIComboable>(weapon->GetJumpDoAction());
		CheckNull(combo);
		combo->DisableCombo();
	}
	else
	{
		IIComboable* combo = Cast<IIComboable>(weapon->GetDoAction());
		CheckNull(combo);
		combo->DisableCombo();
	}
}

//PRIVATE FUNCTION 
void Test()
{
	// ���ӷ��� �ϸ� ��� ���� ��ɿ� ������ ������ ó���ؼ� �ٴڿ� ������� ���� 1 -> �ٴڿ� ���� �� ��Ÿ2 ���� ���� 
	//IIComboable* combo = Cast<IIComboable>(weapon->GetDoAction());
	//CheckNull(combo);
	//combo->DisableCombo();

	//combo = nullptr;
	//combo = Cast<IIComboable>(weapon->GetJumpDoAction());
	//CheckNull(combo);
	//combo->DisableCombo();
}