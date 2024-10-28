#include "Weapons/SubActions/CSubAction_Sword.h"
#include "Global.h"
#include "Characters/CEnemy.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CAttachment.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CWeaponComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"



UCSubAction_Sword::UCSubAction_Sword()
{
	
}

void UCSubAction_Sword::Pressed()
{
	CheckTrue(DoAction->GetInAction()); // �⺻ ���� �߿��� ��� �� �ϰ� ���´�.
	CheckTrue(GetInAction());	// ������ �׼� ���̸�  ó�� 
	CheckFalse(State->IsIdleMode());

	Super::Pressed();

	//�÷� ���� 
	if (ActionDatas.Num() > 0)
	{
		CLog::Log("Sub Action Call");
		ActionDatas[Index].DoAction(Owner);

		State->SetActionMode();
	}
}


void UCSubAction_Sword::OnPressSpecialAction()
{
	CheckFalse(ActionDatas.Num() > 1 );

	CLog::Log("Special Action Call 1 ");

	Index += 1; 
	CheckTrue(ActionDatas.Num() <= Index);

	// ���� Ű ���� ������ ���� �׼� ������.
	if (ActionDatas.Num() > 1)
	{
		CLog::Log("Special Action Call 2");

		StopMovement();

		ActionDatas[Index].DoAction(Owner);
		CLog::Print("Special Action " + FString::FromInt(Index));

		State->SetActionMode();
	}
}



void UCSubAction_Sword::SetInputSubAction()
{

	UInputComponent* input = CHelpers::GetComponent<UInputComponent>(Owner);
	CheckNull(input);
	CLog::Log("Set Special ");

	// ���� Action �̺�Ʈ ���� 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// ���� �׼� ���� �� �̺�Ʈ�� �����ϱ�
	input->BindAction("Action", EInputEvent::IE_Pressed, this, 
		&UCSubAction_Sword::OnPressSpecialAction);
}

void UCSubAction_Sword::EndInputSubAction()
{
	UInputComponent* input = CHelpers::GetComponent<UInputComponent>(Owner);
	CheckNull(input);
	CLog::Log("End Special ");

	// ���� Action �̺�Ʈ ���� 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// ���� �׼� ���� �� �̺�Ʈ�� �����ϱ�
	input->BindAction("Action", EInputEvent::IE_Pressed, Weapon,
		&UCWeaponComponent::DoAction);
}

void UCSubAction_Sword::Begin_DoSubAction_Implementation()
{
	Super::Begin_DoSubAction_Implementation();



	Attachment->OnAttachmentEndCollision.Remove(DoAction, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(DoAction, "OnAttachmentBeginOverlap");

	Attachment->OnAttachmentEndCollision.AddDynamic(this, &UCSubAction_Sword::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(this, &UCSubAction_Sword::OnAttachmentBeginOverlap);

	//TraceAttackArea();
}

void UCSubAction_Sword::End_DoSubAction_Implementation()
{
	Super::End_DoSubAction_Implementation();

	bInAction = false;

	

	Attachment->OnAttachmentEndCollision.Remove(this, "OnAttachmentEndCollision");
	Attachment->OnAttachmentBeginOverlap.Remove(this, "OnAttachmentBeginOverlap");

	CheckNull(DoAction);
	CheckNull(Attachment);

	Attachment->OnAttachmentEndCollision.AddDynamic(DoAction, &UCDoAction::OnAttachmentEndCollision);
	Attachment->OnAttachmentBeginOverlap.AddDynamic(DoAction, &UCDoAction::OnAttachmentBeginOverlap);

	if (Index >= ActionDatas.Num())
	{
		ChangeState();
	}
}


void UCSubAction_Sword::OnAttachmentEndCollision()
{
	Hitted.Empty();
	//Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
}

void UCSubAction_Sword::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther)
{
	CheckNull(InOther);

	for (ACharacter* hitted : Hitted)
		CheckTrue(hitted == InOther);


	Hitted.AddUnique(InOther);

	// ���� ����.
	CLog::Print("Upper Attack Hit!", -1, 1.0f, FColor::Yellow);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

	// ���� ������ ������ ���� �޺� Ÿ�̸� ����
	// 0.1�ʸ��� �� ��ġ ���� 
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
	Owner->GetWorld()->GetTimerManager().SetTimer(TrackEnemyTimeHandle, this, &UCSubAction_Sword::TrackEnemyHeight, 0.05f, true);

	SetInputSubAction();
}

// �� ��ġ ����
void UCSubAction_Sword::TrackEnemyHeight()
{

	// ���Ǿ� Ʈ���̽��� �ֺ� ���� ���� 
	TArray<FHitResult> HitResults;
	FVector StartLocation = Owner->GetActorLocation();
	StartLocation.Z += TrackHeightValue;
	FVector EndLocation = StartLocation;  // ���Ǿ�� ���۰� ���� �����ص� ��

	float SphereRadius = 300.0f;  // ���� �ݰ�
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Owner);

	bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(Owner->GetWorld(),
		StartLocation, EndLocation, SphereRadius, "Targeting",
		false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResults,
		true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	if (bHit && HitResults.Num() > 0)
	{
		for (const FHitResult& hitResult : HitResults)
		{
			ACharacter* enemy = Cast<ACharacter>(hitResult.GetActor());
			if (enemy == NULL)
				continue;
				
			if (enemy->GetActorLocation().Z >= TrackHeightValue)
			{
				TeleportToEnemy(enemy);
				break;
			}
		}
	}

}

// ĳ���� ���� �̵�
void UCSubAction_Sword::TeleportToEnemy(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
	
	
	FVector TargetLocation = InTargetEnemy->GetActorLocation();
	FVector ForwardVector = InTargetEnemy->GetActorForwardVector();

	// ĳ���͸� ���� ���� ��ġ ������ ���� �̵�
	FVector NewLocation = TargetLocation + (ForwardVector * OffsetDistance);
	Owner->SetActorLocation(NewLocation);
	
	StopMovement();

	// �� ��ġ�� ���߱� 
	StopEnemyMovement(InTargetEnemy);
}

// ���� ������ ���߱�
void UCSubAction_Sword::StopEnemyMovement(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);

	// ���� ������ ������Ʈ�� ��������
	UCharacterMovementComponent* MovementComponent = InTargetEnemy->GetCharacterMovement();
	if (MovementComponent)
	{
		// �ӵ��� 0���� �����Ͽ� ���߰� ��
		MovementComponent->StopMovementImmediately();

		// �߰������� ������ ���·� �����
		MovementComponent->SetMovementMode(EMovementMode::MOVE_None); // �Ǵ� ���ϴ� ���� ����

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, InTargetEnemy]()
		{
			ChangeEnemyState(InTargetEnemy);
		});


		Owner->GetWorld()->GetTimerManager().SetTimer(EnemyStateChangeHandle, TimerDelegate, 2.0f, false);
	}
}

// ���� ���� ��ȯ
void UCSubAction_Sword::ChangeEnemyState(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);

	// ���� ������ ������Ʈ�� ��������
	UCharacterMovementComponent* MovementComponent = InTargetEnemy->GetCharacterMovement();

	CheckNull(MovementComponent);

	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
}

void UCSubAction_Sword::StopMovement()
{
	UCharacterMovementComponent* MovementComponent  = Owner->GetCharacterMovement();
	CheckNull(MovementComponent);

	// �ӵ��� 0���� �����Ͽ� ���߰� ��
	MovementComponent->StopMovementImmediately();

	MovementComponent->SetMovementMode(EMovementMode::MOVE_None);

	Owner->GetWorld()->GetTimerManager().ClearTimer(AirSlashTimeHandle);
	Owner->GetWorld()->GetTimerManager().SetTimer(AirSlashTimeHandle, this, &UCSubAction_Sword::ChangeState, 2.0f, false);
}

void UCSubAction_Sword::ChangeState()
{
	EndInputSubAction();
	Index = 0;
	UCharacterMovementComponent* MovementComponent = Owner->GetCharacterMovement();
	CheckNull(MovementComponent);

	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
}

void UCSubAction_Sword::TraceAttackArea()
{

	// ���Ǿ� Ʈ���̽��� �ֺ� ���� ���� 
	TArray<FHitResult> HitResults;
	FVector StartLocation = Owner->GetActorLocation();
	FVector forward = Owner->GetActorForwardVector() * 100.0f;
	StartLocation += forward;
	FVector EndLocation = StartLocation;  // ���Ǿ�� ���۰� ���� �����ص� ��

	float SphereRadius = 150.0f;  // ���� �ݰ�
	TArray<AActor*> IgnoredActors;
	IgnoredActors.Add(Owner);

	bool bHit = UKismetSystemLibrary::SphereTraceMultiByProfile(Owner->GetWorld(),
		StartLocation, EndLocation, SphereRadius, "Targeting",
		false, IgnoredActors, EDrawDebugTrace::ForDuration, HitResults,
		true, FLinearColor::Green, FLinearColor::Red, 1.0f);

	if (bHit && HitResults.Num() > 0)
	{
		for (const FHitResult& hitResult : HitResults)
		{
			ACharacter* enemy = Cast<ACharacter>(hitResult.GetActor());
			if (enemy == NULL || Hitted.Contains(enemy))
				continue;

			Hitted.Add(enemy);
			
			//CLog::Print("Damged Target " + enemy->GetName());
			HitDatas[Index].SendDamage(Owner, Attachment, enemy);

		}
	}
}

void UCSubAction_Sword::CreateAttackSlash()
{
	if (SlashDatas.Num() > 0)
	{
		SlashDatas[Index].CreateSlashEffect(Owner);
	}
}

void FSlashData::CreateSlashEffect(class ACharacter* InOwner)
{
	CheckNull(InOwner);
	CheckNull(AttackEffect);

	FVector ownerLocation = InOwner->GetActorLocation();
	FVector ownerForward = InOwner->GetActorForwardVector() * forward;
	FVector spwanLocation = ownerLocation + ownerForward;

	FRotator spawnRotator = InOwner->GetActorRotation();
	spawnRotator.Roll = angle;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		InOwner->GetWorld(), 
		AttackEffect, 
		spwanLocation, 
		spawnRotator, 
		Scale
	);
}
