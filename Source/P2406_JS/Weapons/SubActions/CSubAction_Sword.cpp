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
	CheckTrue(DoAction->GetInAction()); // 기본 공격 중에도 사용 못 하게 막는다.
	CheckTrue(GetInAction());	// 기존에 액션 중이면  처리 
	CheckFalse(State->IsIdleMode());

	Super::Pressed();

	//올려 베기 
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

	// 공격 키 누를 때마다 공중 액션 실행함.
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

	// 기존 Action 이벤트 제거 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// 서브 액션 중일 때 이벤트로 변경하기
	input->BindAction("Action", EInputEvent::IE_Pressed, this, 
		&UCSubAction_Sword::OnPressSpecialAction);
}

void UCSubAction_Sword::EndInputSubAction()
{
	UInputComponent* input = CHelpers::GetComponent<UInputComponent>(Owner);
	CheckNull(input);
	CLog::Log("End Special ");

	// 기존 Action 이벤트 제거 
	input->RemoveActionBinding("Action", EInputEvent::IE_Pressed);

	// 서브 액션 중일 때 이벤트로 변경하기
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
	CLog::Print("Upper Attack Hit!", -1, 1.0f, FColor::Yellow);
	HitDatas[Index].SendDamage(InAttacker, InAttackCauser, InOther);

	// 적을 공격을 했으면 공중 콤보 타이머 시작
	// 0.1초마다 적 위치 추적 
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
	Owner->GetWorld()->GetTimerManager().SetTimer(TrackEnemyTimeHandle, this, &UCSubAction_Sword::TrackEnemyHeight, 0.05f, true);

	SetInputSubAction();
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

}

// 캐릭터 순간 이동
void UCSubAction_Sword::TeleportToEnemy(ACharacter* InTargetEnemy)
{
	CheckNull(InTargetEnemy);
	Owner->GetWorld()->GetTimerManager().ClearTimer(TrackEnemyTimeHandle);
	
	
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
	UCharacterMovementComponent* MovementComponent  = Owner->GetCharacterMovement();
	CheckNull(MovementComponent);

	// 속도를 0으로 설정하여 멈추게 함
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
