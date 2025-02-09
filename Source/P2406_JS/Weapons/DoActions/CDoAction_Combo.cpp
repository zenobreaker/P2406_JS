#include "Weapons/DoActions/CDoAction_Combo.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"



void UCDoAction_Combo::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);
	if (bEnable)
	{
		bEnable = false;
		bExist = true; 

		return; 
	}

	CheckFalse(State->IsIdleMode());

	Super::DoAction();
	DoActionDatas[Index].DoAction(OwnerCharacter);
}


void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);

	bExist = false;

	DoActionDatas[Index].Destroy_GhostTrail();
	DoActionDatas[++Index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();

	DoActionDatas[Index].Destroy_GhostTrail();
	Index = 0;
}

void UCDoAction_Combo::OnAttachmentEndCollision()
{
	Super::OnAttachmentEndCollision();

	ACharacter* candidate = nullptr;
	candidate = FindBestTarget();

	if (!!candidate)
	{
		FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());

		APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
		if (!!controller)
		{
			FRotator controlRotation = controller->GetControlRotation();
			FRotator target = FRotator(controlRotation.Pitch, rotator.Yaw, controlRotation.Roll);

			controller->SetControlRotation(target);
		}
	}

	Hitted.Empty();
}

void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	CheckTrue(IsMyTeam(InAttacker, InOther)); 

	CheckTrue(IsOtherIsMe(InOther));

	Hitted.AddUnique(InOther);


	CheckTrue(HitDatas.Num() - 1 < Index);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

	// 처리된 캐릭터에 대한 플래그 설정
	InOther->Tags.Add(FName("HitByWeapon"));
}

void UCDoAction_Combo::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{

}
