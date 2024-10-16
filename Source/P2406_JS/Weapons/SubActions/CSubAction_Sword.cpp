#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"

UCSubAction_Sword::UCSubAction_Sword()
{
}

void UCSubAction_Sword::Pressed()
{
	CheckTrue(DoAction->GetInAction()); // 기본 공격 중에도 사용 못 하게 막는다.
	CheckTrue(GetInAction());	// 기존에 액션 중이면  처리 
	CheckFalse(State->IsIdleMode());

	Super::Pressed();

	if (ActionDatas.Num() > 0)
	{
		CLog::Log("Sub Action Call");
		ActionDatas[this->Index].DoAction(Owner);

		State->SetActionMode();
	}
}

void UCSubAction_Sword::Begin_DoSubAction_Implementation()
{
	Super::Begin_DoSubAction_Implementation();

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSubAction_Sword::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword::OnAttachmentBeginOverlap);
}

void UCSubAction_Sword::End_DoSubAction_Implementation()
{
	Super::End_DoSubAction_Implementation();

	bInAction = false;

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

}


void UCSubAction_Sword::OnAttachmentEndCollision()
{
	Hitted.Empty();
}

void UCSubAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);


	Hitted.AddUnique(InOther);

	// 적을 띄운다.
	CLog::Print("Upper Attack Hit!", -1, 1.0f, FColor::Yellow);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);
}


