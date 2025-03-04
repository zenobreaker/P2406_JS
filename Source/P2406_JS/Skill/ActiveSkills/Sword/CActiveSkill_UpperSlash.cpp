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

	// Begin_Charing 스테이트 추가 
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
	
	// 이전 페이즈가 반드시 Begin
	if (CurrentPhase != ESkillPhase::Begin_Charging)
	{
		FLog::Log("Failed Charging");
		return;
	}

	// 뗐을 때 페이즈 전환
	ExecutePhase(ESkillPhase::End_Charging);
}


void UCActiveSkill_UpperSlash::Begin_Charging()
{
	// 충전 실시 
	StartCharing();


	// 여기선 딱히 스테이트를 옮기지 않고 여기에 묶어둔다. 	
	if (SkillPhaseTable.Contains(ESkillPhase::Begin_Charging) && SkillPhaseTable[ESkillPhase::Begin_Charging].PhaseDatas.Num() > 0)
		SkillPhaseTable[ESkillPhase::Begin_Charging].PhaseDatas[0].ExecutePhase(OwnerCharacter);

	//REGISTER_EVENT_WITH_REPLACE(this, OnSkillCastingCompleted, this, UCActiveSkill_UpperSlash::OnSkillCastingCompleted_JudgementBlade);
}

void UCActiveSkill_UpperSlash::End_Charging()
{
	// 공격이 갈리는 기술 결정하고 바로 다음 스테이트로
	FLog::Log("End_Charging- UpperSlash");

	// 1. 제한된 충전 시간 내로 입력 못한 경우 
	if (ChargeTime < MaxChargeTime)
	{
		// 바로 빠른 어퍼슬래쉬
		CurrentState = UpperSlashState::UpperSlash;

	}
	// 2. 충전 완료된 경우 
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

	// ㅇㅓ퍼 슬래시라면 수행 
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

	// Z 값을 올린다. 
	FVector location = Location;
	location.Z += Output;
	FLog::Print("Upper : " + FString::SanitizeFloat(Output), 5015);

	OwnerCharacter->SetActorLocation(location);
}


/*
void UCSubAction_Sword::Pressed()
{
	CheckTrue(DoAction->GetInAction()); // 기본 공격 중에도 사용 못 하게 막는다.
	CheckFalse(State->IsIdleMode());
	CheckTrue(bSubAction == true);

	Super::Pressed();

	//올려 베기
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

	// 공격 키 누를 때마다 공중 액션 실행함.
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

	// 기존 Action 이벤트 제거
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// 서브 액션 중일 때 이벤트로 변경하기
	input->BindAction("Action", EInputEvent::IE_Pressed, this,
		&UCSubAction_Sword::OnPressSpecialAction);
}

void UCSubAction_Sword::EndInputSubAction()
{
	UInputComponent* input = FHelpers::GetComponent<UInputComponent>(Owner);
	CheckNull(input);

	// 기존 Action 이벤트 제거
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

	// 적을 띄운다.
	//CLog::Print("Upper Attack Hit!", -1, 1.0f, FColor::Yellow);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

	// 적을 공격을 했으면 공중 콤보 타이머 시작
	// 0.1초마다 적 위치 추적
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
	Owner->GetWorld()->GetTimerManager().SetTimer(TrackEnemyTimeHandle, this, &UCSubAction_Sword::TrackEnemyHeight, 0.05f, true);

	//SetInputSubAction();
}

// 적 위치 감지
void UCSubAction_Sword::TrackEnemyHeight()
{

	// 스피어 트레이스로 주변 적들 감지
	TArray<FHitResult> HitResults;
	FVector StartLocation = Owner->GetActorLocation();
	StartLocation.Z += TrackHeightValue;
	FVector EndLocation = StartLocation;  // 스피어는 시작과 끝이 동일해도 됨

	float SphereRadius = 300.0f;  // 감지 반경
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

// 캐릭터 순간 이동
void UCSubAction_Sword::TeleportToEnemy(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);

	bSubAction = true;

	FVector TargetLocation = InTargetEnemy->GetActorLocation();
	FVector ForwardVector = InTargetEnemy->GetActorForwardVector();

	// 캐릭터를 적의 공중 위치 정도로 순간 이동
	FVector NewLocation = TargetLocation + (ForwardVector * OffsetDistance);
	Owner->SetActorLocation(NewLocation);

	StopMovement();

	// 그 위치에 멈추기
	StopEnemyMovement(InTargetEnemy);
}

// 적의 움직임 멈추기
void UCSubAction_Sword::StopEnemyMovement(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);

	// 적의 움직임 컴포넌트를 가져오기
	UCharacterMovementComponent* MovementComponent = InTargetEnemy->GetCharacterMovement();
	if (MovementComponent)
	{
		// 속도를 0으로 설정하여 멈추게 함
		MovementComponent->StopMovementImmediately();

		// 추가적으로 고정된 상태로 만들기
		MovementComponent->SetMovementMode(EMovementMode::MOVE_None); // 또는 원하는 모드로 설정

		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([this, InTargetEnemy]()
		{
			ChangeEnemyState(InTargetEnemy);
		});


		Owner->GetWorld()->GetTimerManager().SetTimer(EnemyStateChangeHandle, TimerDelegate, 2.0f, false);
	}
}

// 적의 상태 변환
void UCSubAction_Sword::ChangeEnemyState(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);

	// 적의 움직임 컴포넌트를 가져오기
	UCharacterMovementComponent* MovementComponent = InTargetEnemy->GetCharacterMovement();

	CheckNull(MovementComponent);

	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
}

void UCSubAction_Sword::StopMovement()
{
	UCharacterMovementComponent* MovementComponent = Owner->GetCharacterMovement();
	CheckNull(MovementComponent);

	// 속도를 0으로 설정하여 멈추게 함
	MovementComponent->StopMovementImmediately();

	MovementComponent->SetMovementMode(EMovementMode::MOVE_None);

	Owner->GetWorld()->GetTimerManager().ClearTimer(AirSlashTimeHandle);
	Owner->GetWorld()->GetTimerManager().SetTimer(AirSlashTimeHandle, this, &UCSubAction_Sword::ChangeState, 2.0f, false);
}

void UCSubAction_Sword::ChangeState()
{
	UCharacterMovementComponent* MovementComponent = Owner->GetCharacterMovement();
	CheckNull(MovementComponent);

	// 이미 착지한 상태면 넘긴다.
	if (MovementComponent->MovementMode == EMovementMode::MOVE_Walking)
		return;

	// 착지 상태로 돌리고 모든 상태를 원위치
	MovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);

	EndInputSubAction();
	Index = 0;
	bSubAction = false;

	State->SetIdleMode();
}

void UCSubAction_Sword::TraceAttackArea()
{

	// 스피어 트레이스로 주변 적들 감지
	TArray<FHitResult> HitResults;
	FVector StartLocation = Owner->GetActorLocation();
	FVector forward = Owner->GetActorForwardVector() * 100.0f;
	StartLocation += forward;
	FVector EndLocation = StartLocation;  // 스피어는 시작과 끝이 동일해도 됨

	float SphereRadius = 150.0f;  // 감지 반경
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

