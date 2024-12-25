#include "Weapons/SubActions/CSubAction_Sword_Guard.h"
#include "Global.h"

#include "Characters/CBaseCharacter.h"
#include "Characters/CPlayer.h"

#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"

#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"

#include "Weapons/CAttachment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction.h"


UCSubAction_Sword_Guard::UCSubAction_Sword_Guard()
{

}

void UCSubAction_Sword_Guard::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	Movement = FHelpers::GetComponent<UCMovementComponent>(InOwner);
	State = FHelpers::GetComponent<UCStateComponent>(InOwner);
	if (!!State)
		State->OnStateTypeChanged.AddDynamic(this, &UCSubAction_Sword_Guard::OnStateTypeChanged);
}

void UCSubAction_Sword_Guard::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

}

void UCSubAction_Sword_Guard::OffGuardStance()
{
	Owner->StopAnimMontage(GuardMontage);

	bIsDamaged = false; 
	bIsGuard = false; 

	CheckNull(Movement);

	Movement->Move();
	if (!!State)
		State->SetIdleMode();
}

void UCSubAction_Sword_Guard::Pressed()
{
	CheckNull(Owner);
	CheckNull(State);

	Super::Pressed();

	bIsDamaged = false; 
	

	if (State->IsDamagedMode())
		return;

	State->SetGuardMode();

	Owner->PlayAnimMontage(GuardMontage, PlayRate);

	CheckNull(Movement);
	Movement->Stop();
}

void UCSubAction_Sword_Guard::Released()
{
	OffGuardStance();
}


// �� �Լ��� ȣ��Ǿ��ٴ� �� �������� ���Դٴ� �� 
bool UCSubAction_Sword_Guard::TryGuard(ACBaseCharacter::FDamageData& DamageData)
{
	CheckTrueResult(Owner == nullptr, false);

	bIsDamaged = true;

	auto InAttackLocation  = DamageData.Attacker->GetActorLocation();

	FVector toAttack = (InAttackLocation - Owner->GetActorLocation()).GetSafeNormal();
	FVector forward = Owner->GetActorForwardVector();

	float DotProduct = FVector::DotProduct(forward, toAttack);

	DebugLine(toAttack, forward);

	if (DotProduct > FMath::Cos(FMath::DegreesToRadians(GuardAngle)))
	{
		UGameplayStatics::PlaySoundAtLocation(Owner->GetWorld(), GuardSound, Owner->GetActorLocation());

		// ����Ʈ Ÿ�̹� ȣ���غ�
		Evaluate_JustTiming(DamageData.Attacker);

		return true;
	}

	return Super::TryGuard(DamageData);
}

void UCSubAction_Sword_Guard::OnGuard()
{
	bIsGuard = !bIsGuard;

	FString debug = (bIsGuard == true) ? "On" : "Off";

	FLog::Print("Guard Time " + debug);
}

void UCSubAction_Sword_Guard::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
	case EStateType::Damaged:
		OffGuardStance();
		break;
	default:
		break;
	}
}

void UCSubAction_Sword_Guard::Begin_DoSubAction_Implementation()
{
	Super::Begin_DoSubAction_Implementation();

	ACPlayer* player = Cast<ACPlayer>(Owner); 
	if (!!player)
		player->SetCanInput(false);

	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSubAction_Sword_Guard::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword_Guard::OnAttachmentBeginOverlap);

	
}

void UCSubAction_Sword_Guard::End_DoSubAction_Implementation()
{
	Super::End_DoSubAction_Implementation();

	ACPlayer* player = Cast<ACPlayer>(Owner);
	if (!!player)
		player->SetCanInput(true);

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	CheckNull(DoAction);
	CheckNull(Attachment);

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);
}

void UCSubAction_Sword_Guard::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	FLog::Print("Guard Parry call");
	CheckNull(InOther);

	for (ACharacter* hit : Hitted)
		CheckTrue(hit == InOther);

	Hitted.AddUnique(InOther);

	FLog::Print("Guard Parry Send Damage");
	HitData.SendDamage(InAttacker, InAttackCauser, InOther);
}

void UCSubAction_Sword_Guard::OnAttachmentEndCollision()
{
	Hitted.Empty();
}

void UCSubAction_Sword_Guard::Evaluate_JustTiming(AActor* InAttacker)
{
	CheckNull(InAttacker);


	bDebug = true; 
	// �� ������ ����� Ÿ�̹��� �� �Լ��� ȣ��� Ÿ�̹��̶��? 
	if (bIsGuard || bDebug)
	{
		ACharacter* character = Cast<ACharacter>(InAttacker);
		
		if (!!character)
		{
			FVector start = Owner->GetActorLocation();
			FVector target = InAttacker->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			character->LaunchCharacter(-direction * 200.0f, false, false);
		}
		FLog::Print("Just Timing!! ", -1, 5.0f, FColor::Yellow);
		//TODO: ���� �����ڿ��� �� �и����������ϱ� �� �и������! �ؾ��Ѵ�
		//Owner->PlayAnimMontage(ParringAttackMontage);
		ActionData.DoAction(Owner);
		//TODO : ������ �����ڰ� �и����ϴ� ���¿� ���� ó���� �ȵǾ� �־ ������ ���� �������Ҽ��ִ�
	}
}

void UCSubAction_Sword_Guard::Evaluate_GuradStance()
{
}


void UCSubAction_Sword_Guard::DebugLine(FVector InAttack, FVector InForward)
{
	DrawDebugLine(
		Owner->GetWorld(),
		Owner->GetActorLocation(),
		Owner->GetActorLocation() + InForward * 300.0f,
		FColor::Blue,
		false,
		1.0f);

	DrawDebugLine(
		Owner->GetWorld(),
		Owner->GetActorLocation(),
		Owner->GetActorLocation() + InAttack * 300.0f,
		FColor::Red,
		false,
		1.0f);

	FRotator rotator = FRotator(0, Owner->GetControlRotation().Yaw, 0);
	FVector n_forward = FQuat(rotator).GetForwardVector();

	FVector leftDirection = n_forward.RotateAngleAxis(-GuardAngle, Owner->GetActorUpVector()); // ���� ����
	FVector rightDirection = n_forward.RotateAngleAxis(GuardAngle, Owner->GetActorUpVector()); // ������ ����

	DrawDebugLine(
		Owner->GetWorld(),
		Owner->GetActorLocation(),
		Owner->GetActorLocation() + leftDirection * 300.0f,
		FColor::Green,
		false,
		1.0f);



	DrawDebugLine(
		Owner->GetWorld(),
		Owner->GetActorLocation(),
		Owner->GetActorLocation() + rightDirection * 300.0f,
		FColor::Green,
		false,
		1.0f);

}

void UCSubAction_Sword_Guard::SetDebug()
{
	bDebug = !bDebug;
}
