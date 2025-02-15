#include "Weapons/DoActions/CDoAction_JumpAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "Characters/CBaseCharacter.h"
#include "Characters/IDamagable.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"

#include "Weapons/CAttachment.h"

#include "GenericTeamAgentInterface.h"

void UCDoAction_JumpAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas)
{
	Super::BeginPlay(InOwner, InAttachment, InEquipment, InDoActionDatas, InHitDatas);

	CheckNull(OwnerCharacter);

	ACBaseCharacter* base = Cast<ACBaseCharacter>(OwnerCharacter);
	CheckNull(base);

	REGISTER_EVENT_WITH_REPLACE(base, OnCharacterLanded, this, UCDoAction_JumpAction::SetFallMode);
	REGISTER_EVENT_WITH_REPLACE(base, OnCharacterLanded, this, UCDoAction_JumpAction::Lanaded_FallAttack);

	Weapon = FHelpers::GetComponent<UCWeaponComponent>(base);
}

void UCDoAction_JumpAction::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCEquipment* InEquipment, const TArray<FDoActionData>& InDoActionDatas, const TArray<FHitData>& InHitDatas, const TArray<FDoActionData>& InFallActionDatas, const TArray<FHitData>& InFallHitDatas)
{
	BeginPlay(InOwner, InAttachment, InEquipment, InDoActionDatas, InHitDatas);

	FallActionDatas = InFallActionDatas;
	FallHitDatas = InHitDatas;
}

void UCDoAction_JumpAction::Tick(float InDelaTime)
{
	Super::Tick(InDelaTime);

	switch (FAState)
	{
		case FallAttackState::Begin: break;
		case FallAttackState::Loop: break;
		case FallAttackState::End:
		{
			// ĳ���Ͱ� ���� �����ϴ��� Ʈ���̽� ���� �˻� 
		}
		break;
		case FallAttackState::Max: break;
	}
}

void UCDoAction_JumpAction::InitializeFallAttackData(
	const TArray<FDoActionData>& InFallActionDatas,
	const TArray<FHitData>& InHitDatas)
{
	FallActionDatas = InFallActionDatas;
	FallHitDatas = InHitDatas;
}

void UCDoAction_JumpAction::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);
	CheckTrue(bInExtraAction);

	// ���� ���� �Է��� �����ִٸ� ���� ����
	if (bIsFallAttack)
	{
		bInExtraAction = true;
		//FLog::Print("Fall Attack!");
		DoAction_FallAttackFlow(FallAttackState::Begin);

		return;
	}


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

void UCDoAction_JumpAction::Begin_DoAction()
{
	Super::Begin_DoAction();

	// ���� ���� �Է��� �����ִٸ� ���� ����
	if (bIsFallAttack)
	{
		Hitted.Empty();

		//FLog::Print("Fall Attack!");
		Begin_DoAction_FallAttack();

		return;
	}

	CheckFalse(bExist);

	bExist = false;

	DoActionDatas[++Index].DoAction(OwnerCharacter);
	SetFlyMode();
}

void UCDoAction_JumpAction::End_DoAction()
{
	Super::End_DoAction();

	End_DoAction_FallAttack();
	
	DoActionDatas[Index].End_DoAction(OwnerCharacter);

	Index = 0;
	SetFallMode();
}

void UCDoAction_JumpAction::SetHeavyActionFlag(bool InValue)
{
	Super::SetHeavyActionFlag(InValue);
	bIsFallAttack = InValue;
}

void UCDoAction_JumpAction::OnAttachmentEndCollision()
{
	Super::OnAttachmentBeginCollision();

	Hitted.Empty();
}

void UCDoAction_JumpAction::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther);
	CheckNull(InOther);

	CheckTrue(IsMyTeam(InAttacker, InOther));

	CheckTrue(IsOtherIsMe(InOther));

	Hitted.AddUnique(InOther);


	CheckTrue(HitDatas.Num() - 1 < Index);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther, Hitted.Num() <= 1);

	// ó���� ĳ���Ϳ� ���� �÷��� ����
	InOther->Tags.Add(FName("HitByWeapon"));
}

void UCDoAction_JumpAction::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{

}

//-----------------------------------------------------------------------------
/// <summary>
/// Fall Attack Func
/// </summary>
void UCDoAction_JumpAction::DoAction_FallAttackFlow(FallAttackState InState)
{
	FAState = InState;
	switch (InState)
	{
		case FallAttackState::Begin:
		{
			SetFlyMode();

			PreviousCollisionResponse = OwnerCharacter->GetCapsuleComponent()->GetCollisionResponseToChannel(ECC_Pawn);
			OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
			// ���� ��� ���� 
			FallActionDatas[(int32)FallAttackState::Begin].DoAction(OwnerCharacter);

			Gravity = OwnerCharacter->GetCharacterMovement()->GravityScale;
			Velocity = OwnerCharacter->GetCharacterMovement()->Velocity.Z;

		}
		break;
		case FallAttackState::Loop:
		{
			// ���� ���� 
			SetFallMode();
			FallActionDatas[(int32)FallAttackState::Loop].DoAction(OwnerCharacter);
			Begin_DoAction_FallAttack();
		}
		break;
		case FallAttackState::End:
		{
			// ���� �� ��� ����
			// ���� ã�� �ߴٸ� ����� �߻�
			FallActionDatas[(int32)FallAttackState::End].DoAction(OwnerCharacter);

			// ��� ������ �ʱ�ȭ�Ѵ�. 
			FAState = FallAttackState::Max;
			bIsFallAttack = false;
			bInExtraAction = false;
			bEnable = false;
			bExist = false;

			OwnerCharacter->GetCharacterMovement()->Velocity.Z = Velocity;
			OwnerCharacter->GetCharacterMovement()->GravityScale = Gravity;
			Index = 0;

			if (Weapon != nullptr)
				Weapon->SetFallingAttack(false);

			OwnerCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, PreviousCollisionResponse);

			ChecFallAttackHit();
		}
		break;
		case FallAttackState::Max:
		break;
	}
}


void UCDoAction_JumpAction::Begin_DoAction_FallAttack()
{
	switch (FAState)
	{
		case FallAttackState::Begin:
		{
			// �Է��� �� ���� �����Ѵٰ� �÷��� ON
			if (Weapon != nullptr)
				Weapon->SetFallingAttack(true);
		
			// ������ �ű�
			DoAction_FallAttackFlow(FallAttackState::Loop);

		}
		break;
		case FallAttackState::Loop:
		{
			OwnerCharacter->GetCharacterMovement()->GravityScale = Gravity * 4.0f;
			OwnerCharacter->GetCharacterMovement()->Velocity.Z = -3000;
		}
		break;
		case FallAttackState::End:
		{
		
		}
		break;
		case FallAttackState::Max:
		break;
	}
}

void UCDoAction_JumpAction::End_DoAction_FallAttack()
{
	switch (FAState)
	{
		case FallAttackState::Begin:
		{
		}
		break;
		case FallAttackState::Loop:
		{

		}
		break;
		case FallAttackState::End:
		{
			FAState = FallAttackState::Max;
		}
		break;
		case FallAttackState::Max:
		break;
	}
}

void UCDoAction_JumpAction::PlayFallAttackMontage()
{
	FallActionDatas[(int32)FallAttackState::Loop].DoAction(OwnerCharacter);
}

//-----------------------------------------------------------------------------


/// <summary>
///  ���� ���� ���� 
/// </summary>
void UCDoAction_JumpAction::ChecFallAttackHit()
{
	CheckNull(OwnerCharacter);


	TArray<FHitResult> hitResults;
	FVector location = OwnerCharacter->GetActorLocation();
	location.Z = -OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	// Ʈ���̽� ���� 
	FCollisionQueryParams tracePramams;
	tracePramams.AddIgnoredActor(OwnerCharacter);

	FCollisionObjectQueryParams objectQueryParams;
	objectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	objectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	objectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	float radius = 300.0f;

	// Ʈ���̽� ����
	bool bHit = OwnerCharacter->GetWorld()->SweepMultiByObjectType(
		hitResults,
		location,
		location,
		FQuat::Identity,
		objectQueryParams,  // Ȥ�� ECC_GameTraceChannel1 �� ���ϴ� ä�� ���
		FCollisionShape::MakeSphere(radius),
		tracePramams
	);

	DrawDebugSphere(OwnerCharacter->GetWorld(), location, radius, 12, FColor::Red, false, 1.0f);

	// �浹 �߻� �� ó��
	if (bHit)
	{
		for (FHitResult& hit : hitResults)
		{
			AActor* HitActor = hit.GetActor();
			ACharacter* character = Cast<ACharacter>(HitActor);
			if (character == nullptr)
				continue;

			if (Hitted.Find(character) != INDEX_NONE)
				continue;

			Hitted.AddUnique(character);

			FallHitDatas[0].SendDamage(OwnerCharacter, Weapon->GetAttachment(), character, Hitted.Num() <= 1);
		}
	}
}



//-----------------------------------------------------------------------------

void UCDoAction_JumpAction::SetFlyMode()
{
	// �� ��° ���� ���ݺ��� Fly Mode Ȱ��ȭ
	UCharacterMovementComponent* CharMove = OwnerCharacter->GetCharacterMovement();
	CharMove->SetMovementMode(MOVE_Flying);
}

void UCDoAction_JumpAction::SetFallMode()
{
	// ������ ���� Fly������ 
	UCharacterMovementComponent* CharMove = OwnerCharacter->GetCharacterMovement();
	if (CharMove->MovementMode != EMovementMode::MOVE_Flying)
		return;

	CharMove->SetMovementMode(MOVE_Falling);

}

void UCDoAction_JumpAction::Lanaded_FallAttack()
{
	CheckFalse(bInExtraAction);

	DoAction_FallAttackFlow(FallAttackState::End);
}


//ACharacter* candidate = nullptr;
//candidate = FindBestTarget();

//if (!!candidate)
//{
//	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), candidate->GetActorLocation());

//	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();
//	if (!!controller)
//	{
//		FRotator controlRotation = controller->GetControlRotation();
//		FRotator target = FRotator(controlRotation.Pitch, rotator.Yaw, controlRotation.Roll);

//		controller->SetControlRotation(target);
//	}
//}