#include "Skill/ActiveSkills/Sword/CActiveSkill_UpperSlash.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Skill/CSkillEntity.h"
#include "Curves/CurveFloat.h"

UCActiveSkill_UpperSlash::UCActiveSkill_UpperSlash()
{
	FHelpers::GetAsset<UCurveFloat>(&Curve, "/Script/Engine.CurveFloat'/Game/Weapon_Skills/Swords/Skill3_UpperSlash/Sword_Rising_Curve.Sword_Rising_Curve'");
}

void UCActiveSkill_UpperSlash::BeginPlay_ActiveSkill(ACharacter* InOwner, FSkillFlowData InFlowData)
{
	Super::BeginPlay_ActiveSkill(InOwner, InFlowData);

	CurrentState = UpperSlashState::UpperSlash;
	
	FOnTimelineFloat timeline;
	timeline.BindUFunction(this, "OnRising");
	Timeline.AddInterpFloat(Curve, timeline);
	Timeline.SetPlayRate(PlayRate);
	
	CheckNull(InOwner); 
	
	OriginGravityScale = OwnerCharacter->GetCharacterMovement()->GravityScale;
	
	OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(GravityResetHandle);
}

void UCActiveSkill_UpperSlash::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	
	Timeline.TickTimeline(InDeltaTime);
}

void UCActiveSkill_UpperSlash::DefineSkillPhases()
{
	Super::DefineSkillPhases();

	SkillPhaseTable.Empty();

	// Begin_Charing ������Ʈ �߰� 
	AssignSkillPhase(ESkillPhase::Begin_Charging,
		[this]() { Begin_Charging(); });

	AssignSkillPhase(ESkillPhase::End_Charging,
		[this]() { End_Charging(); });

	AssignSkillPhase(ESkillPhase::Begin_Skill,
		[this]() { Begin_Skill(); });

	AssignSkillPhase(ESkillPhase::End_Skill,
		[this]() { End_Skill(); });
}

void UCActiveSkill_UpperSlash::ReleaseSkill()
{
	Super::ReleaseSkill();

	FLog::Log("Release Skill - UpperSlash");

	Location = OwnerCharacter->GetActorLocation();
	
	// ���� ����� �ݵ�� Begin
	if (CurrentPhase != ESkillPhase::Begin_Charging)
	{
		FLog::Log("Failed Charging");
		return;
	}

	// ���� �� ������ ��ȯ
	ExecutePhase(ESkillPhase::End_Charging);
}


void UCActiveSkill_UpperSlash::Begin_Charging()
{
	// ���� �ǽ� 
	StartCharing();


	// ���⼱ ���� ������Ʈ�� �ű��� �ʰ� ���⿡ ����д�. 	
	if (SkillPhaseTable.Contains(ESkillPhase::Begin_Charging) && SkillPhaseTable[ESkillPhase::Begin_Charging].PhaseDatas.Num() > 0)
		SkillPhaseTable[ESkillPhase::Begin_Charging].PhaseDatas[0].ExecutePhase(OwnerCharacter);

	//REGISTER_EVENT_WITH_REPLACE(this, OnSkillCastingCompleted, this, UCActiveSkill_UpperSlash::OnSkillCastingCompleted_JudgementBlade);
}

void UCActiveSkill_UpperSlash::End_Charging()
{
	// ������ ������ ��� �����ϰ� �ٷ� ���� ������Ʈ��
	FLog::Log("End_Charging- UpperSlash");

	// 1. ���ѵ� ���� �ð� ���� �Է� ���� ��� 
	if (ChargeTime < MaxChargeTime)
	{
		// �ٷ� ���� ���۽�����
		CurrentState = UpperSlashState::UpperSlash;

	}
	// 2. ���� �Ϸ�� ��� 
	else
	{
		CurrentState = UpperSlashState::GaleShash;
	}

	ChargeTime = 0.0f;

	ExecutePhase(ESkillPhase::Begin_Skill);
}

void UCActiveSkill_UpperSlash::Begin_Skill()
{
	CheckFalse(SkillPhaseTable.Contains(ESkillPhase::Begin_Skill));
	CheckFalse(SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas.Num() > 0);

	ACSkillEntity* entity = nullptr;

	// ������ �����ö�� ���� 
	if (CurrentState == UpperSlashState::UpperSlash)
	{
		FLog::Log("Begin Skill - UpperSlash");
		entity = SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas[(int32)UpperSlashState::UpperSlash].ExecutePhase(OwnerCharacter);
	}
	else
	{
		FLog::Log("Begin Skill - Gale Slash");
		if (SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas.Num() > 1)
		{
			if (Timeline.IsPlaying())
				Timeline.Stop();

			if (Timeline.IsPlaying() == false)
				Timeline.PlayFromStart();

			OwnerCharacter->GetCharacterMovement()->GravityScale = 0.0f;


			entity = SkillPhaseTable[ESkillPhase::Begin_Skill].PhaseDatas[(int32)UpperSlashState::GaleShash].ExecutePhase(OwnerCharacter);
		}
	}

	if (entity != nullptr)
	{
		SkillEntity = entity;
		REGISTER_EVENT_WITH_REPLACE(this, OnSkillEnded, entity, ACSkillEntity::DestroySkill);
		
		OnActivated_Collision();
	}
}

void UCActiveSkill_UpperSlash::End_Skill()
{
	Timeline.Stop();

	int32 damagedCount = 0; 
	if (SkillEntity != nullptr )
	{
		damagedCount = SkillEntity->GetDamagedCount();
	}

	float airialTime = 2.5f; 

	if (damagedCount > 0)
	{
		airialTime = 4.0f;
	}

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]()
	{
		OwnerCharacter->GetCharacterMovement()->GravityScale = OriginGravityScale;

		OwnerCharacter->GetWorld()->GetTimerManager().ClearTimer(GravityResetHandle);
	});
	OwnerCharacter->GetWorld()->GetTimerManager().SetTimer(GravityResetHandle, timerDelegate, airialTime, false);

	ExecutePhase(ESkillPhase::Finished);
}

void UCActiveSkill_UpperSlash::OffSkillDoAction()
{

	ExecutePhase(ESkillPhase::End_Skill);
}

void UCActiveSkill_UpperSlash::OnHoveringTarget(ACharacter* InOther)
{

}

void UCActiveSkill_UpperSlash::OnRising(float Output)
{
	CheckNull(OwnerCharacter);

	// Z ���� �ø���. 
	FVector location = Location;
	location.Z += Output;
	FLog::Print("Upper : " + FString::SanitizeFloat(Output), 5015);

	OwnerCharacter->SetActorLocation(location);
}


/*
void UCSubAction_Sword::Pressed()
{
	CheckTrue(DoAction->GetInAction()); // �⺻ ���� �߿��� ��� �� �ϰ� ���´�.
	CheckFalse(State->IsIdleMode());
	CheckTrue(bSubAction == true);

	Super::Pressed();

	//�÷� ����
	if (ActionDatas.Num() > 0)
	{
		ActionDatas[Index].DoAction(Owner);

		State->SetActionMode();
	}
}


void UCSubAction_Sword::OnPressSpecialAction()
{
	CheckFalse(ActionDatas.Num() > 1);

	Index += 1;
	CheckTrue(ActionDatas.Num() <= Index);

	// ���� Ű ���� ������ ���� �׼� ������.
	if (ActionDatas.Num() > 1)
	{

		StopMovement();

		ActionDatas[Index].DoAction(Owner);

		State->SetActionMode();
	}
}



void UCSubAction_Sword::SetInputSubAction()
{

	UInputComponent* input = FHelpers::GetComponent<UInputComponent>(Owner);
	CheckNull(input);

	// ���� Action �̺�Ʈ ����
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// ���� �׼� ���� �� �̺�Ʈ�� �����ϱ�
	input->BindAction("Action", EInputEvent::IE_Pressed, this,
		&UCSubAction_Sword::OnPressSpecialAction);
}

void UCSubAction_Sword::EndInputSubAction()
{
	UInputComponent* input = FHelpers::GetComponent<UInputComponent>(Owner);
	CheckNull(input);

	// ���� Action �̺�Ʈ ����
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	//
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
	//CLog::Print("Upper Attack Hit!", -1, 1.0f, FColor::Yellow);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

	// ���� ������ ������ ���� �޺� Ÿ�̸� ����
	// 0.1�ʸ��� �� ��ġ ����
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
	Owner->GetWorld()->GetTimerManager().SetTimer(TrackEnemyTimeHandle, this, &UCSubAction_Sword::TrackEnemyHeight, 0.05f, true);

	//SetInputSubAction();
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

	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
}

// ĳ���� ���� �̵�
void UCSubAction_Sword::TeleportToEnemy(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);

	bSubAction = true;

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
	UCharacterMovementComponent* MovementComponent = Owner->GetCharacterMovement();
	CheckNull(MovementComponent);

	// �ӵ��� 0���� �����Ͽ� ���߰� ��
	MovementComponent->StopMovementImmediately();

	MovementComponent->SetMovementMode(EMovementMode::MOVE_None);

	Owner->GetWorld()->GetTimerManager().ClearTimer(AirSlashTimeHandle);
	Owner->GetWorld()->GetTimerManager().SetTimer(AirSlashTimeHandle, this, &UCSubAction_Sword::ChangeState, 2.0f, false);
}

void UCSubAction_Sword::ChangeState()
{
	UCharacterMovementComponent* MovementComponent = Owner->GetCharacterMovement();
	CheckNull(MovementComponent);

	// �̹� ������ ���¸� �ѱ��.
	if (MovementComponent->MovementMode == EMovementMode::MOVE_Walking)
		return;

	// ���� ���·� ������ ��� ���¸� ����ġ
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);

	EndInputSubAction();
	Index = 0;
	bSubAction = false;

	State->SetIdleMode();
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
*/

