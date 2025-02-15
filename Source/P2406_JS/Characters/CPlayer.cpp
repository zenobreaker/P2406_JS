#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "CGameMode.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Characters/CBaseCharacter.h"
#include "Characters/CGhostTrail.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CDashComponent.h"
#include "Components/CSkillComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CAirborneComponent.h"
#include "Components/CAttackTraceComponent.h"
#include "Components/CGuardComponent.h"
#include "Components/CZoomComponent.h"

#include "Weapons/CWeaponStructures.h"

#include "Widgets/CUserWidget_Player.h"
#include "Widgets/CUserWidget_SkillHUD.h"

ACPlayer::ACPlayer()
{
	FHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	FHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	CreateActorComponent();

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh = nullptr;
	FHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	FHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	CreateArrowGroup();

	FHelpers::GetAsset<UAnimMontage>(&BackstepMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/BackStep_Montage.BackStep_Montage'");

	FHelpers::GetAsset<UAnimMontage>(&JumpMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Unarmed_JumpStart_Montage.Unarmed_JumpStart_Montage'");

	FHelpers::GetClass<UCUserWidget_Player>(&UiClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");

	FHelpers::GetClass<UCUserWidget_SkillHUD>(&SkillHUDClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/MyCUserWidget_SkillHUD.MyCUserWidget_SkillHUD_C'");
}

void ACPlayer::CreateActorComponent()
{
	FHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	FHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	FHelpers::CreateActorComponent<UCDashComponent>(this, &Dash, "Dash");
	FHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");
	FHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	FHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
	FHelpers::CreateActorComponent<UCGrapplingComponent>(this, &Grapple, "Grapple");
	FHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "Health");
	FHelpers::CreateActorComponent<UCSkillComponent>(this, &Skill, "Skill");
	FHelpers::CreateActorComponent<UCConditionComponent>(this, &Condition, "Condition");
	FHelpers::CreateActorComponent<UCAttackTraceComponent>(this, &ATrace, "A_Trace");
	FHelpers::CreateActorComponent<UCZoomComponent>(this, &Zoom, "Zoom");

	if (!!Grapple)
	{
		Grapple->PrimaryComponentTick.bCanEverTick = true;
	}

	// Guard 인터페이스를 구현했다면 컴포넌트가 자동으로 부착된다.
	if (IIGuardable::Execute_HasGuard(this))
	{
		FHelpers::CreateActorComponent<UCGuardComponent>(this, &Guard, "Guard");
	}
}

void ACPlayer::CreateArrowGroup()
{

	// 애로우 컴포넌트 가져오기 
	FHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "Arrows", GetCapsuleComponent());
	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		FHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name),
			ArrowGroup);

		switch ((EParkourArrowType)i)
		{
		case EParkourArrowType::Center:
			Arrows[i]->ArrowColor = FColor::Red;
			break;
		case EParkourArrowType::Head:
			Arrows[i]->ArrowColor = FColor::Green;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, 100));
			break;

		case EParkourArrowType::Foot:
			Arrows[i]->ArrowColor = FColor::Blue;
			Arrows[i]->SetRelativeLocation(FVector(0, 0, -80));
			break;

		case EParkourArrowType::Left:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, -30, 0));
			break;

		case EParkourArrowType::Right:
			Arrows[i]->ArrowColor = FColor::Magenta;
			Arrows[i]->SetRelativeLocation(FVector(0, +30, 0));
			break;

		case EParkourArrowType::Land:
			Arrows[i]->ArrowColor = FColor::Yellow;
			Arrows[i]->SetRelativeLocation(FVector(200, 0, 100));
			Arrows[i]->SetRelativeRotation(FRotator(-90, 0, 0));
			break;
		}
	}

}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerCameraManager* camearManager = GetController<APlayerController>()->PlayerCameraManager;

	camearManager->ViewPitchMin = PitchAngle.X;
	camearManager->ViewPitchMax = PitchAngle.Y;

	bCountering = new bool();
	*bCountering = false;

	Movement->OnRun();
	Movement->DisableControlRotation();

	if (JumpValue > 0)
	{
		GetCharacterMovement()->JumpZVelocity = JumpValue;
	}


	ensure(Weapon != nullptr);  // Weapon이 nullptr이라면 경고 출력
	ensure(State != nullptr);  // State가 nullptr이라면 경고 출력

	REGISTER_EVENT_WITH_REPLACE(State, OnStateTypeChanged, this, ACPlayer::OnStateTypeChanged);

	REGISTER_EVENT_WITH_REPLACE(Weapon, OnWeaponTypeChanged, this, ACPlayer::OnWeaponTypeChanged);


	// 일반 캐릭터 UI
	if (!!UiClass)
	{
		UserInterface = Cast<UCUserWidget_Player>(CreateWidget(GetController<APlayerController>(), UiClass));
		if (!!UserInterface)
		{
			UserInterface->AddToViewport();
			//UserInterface->UpdateWeaponType(EWeaponType::Max);
			UserInterface->UpdateCrossHairVisibility(false);
			UserInterface->UpdateGuardGaugeVisibility(false);

			REGISTER_EVENT_WITH_REPLACE(Guard, OnUpdatedGuardVisiable, UserInterface, UCUserWidget_Player::UpdateGuardGaugeVisibility);
			REGISTER_EVENT_WITH_REPLACE(Guard, OnUpdatedGuardGauge, UserInterface, UCUserWidget_Player::UpdateGuardGauge);
		}
	}

	// Skill HUD
	if (!!SkillHUDClass)
	{
		SkillHUD = Cast<UCUserWidget_SkillHUD>(CreateWidget(GetController<APlayerController>(), SkillHUDClass));

		REGISTER_EVENT_WITH_REPLACE(Skill, OnSetSkills, SkillHUD, UCUserWidget_SkillHUD::OnSetSkill);
		REGISTER_EVENT_WITH_REPLACE(Skill, OnSkillSlotsCleared, SkillHUD, UCUserWidget_SkillHUD::OnSetSkillSlotsCleared);

		if (!!SkillHUD)
		{
			SkillHUD->AddToViewport();
			SkillHUD->OnSetSkillSlotsCleared();	// 하위 Widget은 화면에 배치되어야 호출됨
		}

		// 게임모드에 이벤트 구독시키기 
		ACGameMode* gameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
		if (!!gameMode)
			gameMode->SubscribeToSkillEvents(SkillHUD);
	}

}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);


	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	PlayerInputComponent->BindAction("Dash", EInputEvent::IE_Pressed, Dash, &UCDashComponent::OnDash);


	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);
	PlayerInputComponent->BindAction("Warp", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetWarpMode);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);
	PlayerInputComponent->BindAction("Action_B", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction_Heavy);

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSubAction);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffSubAction);

	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Pressed, this, &ACPlayer::OnJumpAction);
	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Released, this, &ACPlayer::OnJumpActionEnd);

	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);

	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, Target, &UCTargetComponent::Toggle);
	PlayerInputComponent->BindAction("Target_Left", EInputEvent::IE_Pressed, Target, &UCTargetComponent::MoveLeft);
	PlayerInputComponent->BindAction("Target_Right", EInputEvent::IE_Pressed, Target, &UCTargetComponent::MoveRight);


	if (!!Zoom)
		PlayerInputComponent->BindAxis("Zoom", Zoom, &UCZoomComponent::SetValue);

	//TODO: 일단무식한 방법으로 처리함 나중에 개념 충족하면 여기 수정하기 
	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Pressed, this,
		&ACPlayer::OnSkill1);
	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Released, this,
		&ACPlayer::ReleaseSkill1);

	PlayerInputComponent->BindAction("Skill2", EInputEvent::IE_Pressed, this,
		&ACPlayer::OnSkill2);
	PlayerInputComponent->BindAction("Skill2", EInputEvent::IE_Released, this,
		&ACPlayer::ReleaseSkill2);

	PlayerInputComponent->BindAction("Skill3", EInputEvent::IE_Pressed, this,
		&ACPlayer::OnSkill3);
	PlayerInputComponent->BindAction("Skill3", EInputEvent::IE_Released, this,
		&ACPlayer::ReleaseSkill3);

	PlayerInputComponent->BindAction("Skill4", EInputEvent::IE_Pressed, this,
		&ACPlayer::OnSkill4);
	PlayerInputComponent->BindAction("Skill4", EInputEvent::IE_Released, this,
		&ACPlayer::ReleaseSkill4);

	//////////////////////////////////////////////////////////////////////////////
	// 타임 스케일 조정을 위한 바인딩
	PlayerInputComponent->BindAction("Increase_TimeScale", IE_Pressed, this,
		&ACPlayer::OnIncreaseTimeScale);

	PlayerInputComponent->BindAction("Decrease_TimeScale", IE_Pressed, this,
		&ACPlayer::OnDecreaseTimeScale);

	//////////////////////////////////////////////////////////////////////////////

	ACGameMode* gameMode = GetWorld()->GetAuthGameMode<ACGameMode>();
	PlayerInputComponent->BindAction("State_Down", IE_Pressed, gameMode,
		&ACGameMode::OrderToAllActorDown);

	PlayerInputComponent->BindAction("Toggle_EnemyUI", IE_Pressed, gameMode,
		&ACGameMode::ToggleEnemyUI);

}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	//Guard Check
	{
		// 무기에게 가드 판정 갖고옴 
		bool isGuard = false;
		if (!!Guard)
			isGuard = Guard->CheckBlocking(DamageData);

		if (*bCountering == true)
		{
			//FLog::Print(" Damage in Counter ");

			return Damage;
		}

		if (isGuard)
		{
			//FLog::Print(" Guard Success!!", 2, 5.0f, FColor::Yellow);
			Launch(*DamageData.Event->HitData, true);

			return Damage;
		}
	}

	// Evade Check 
	{
		bool isEvade = false; 
		if (!!Dash)
			isEvade = Dash->IsEvade();
		
		if (isEvade)
			return Damage;
	}

	State->SetDamagedMode();

	return Damage;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	bool bGuardgaugeVisible = false;
	switch (InNewType)
	{
	case EStateType::Evade: Backstep(); break;
	case EStateType::Damaged: Damaged(); break;
	case EStateType::Guard:
	{
		bGuardgaugeVisible = true;
	}
	break;
	}

	if (!!UserInterface)
	{
		UserInterface->UpdateGuardGaugeVisibility(bGuardgaugeVisible);
	}
}

void ACPlayer::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	/*if(!!UserInterface)
		UserInterface->UpdateWeaponType(InNewType);*/

	bool bVisible = false;

	if (InNewType == EWeaponType::Bow)
		bVisible = true;

	if (!!UserInterface)
	{
		//FLog::Print("UserInteface !");
		UserInterface->UpdateCrossHairVisibility(bVisible);
	}
	/*else
	{
		FLog::Print("UserInteface NO");
	}*/
}

void ACPlayer::Launch(const FHitData& InHitData, const bool bIsGuarding)
{
	Super::Launch(InHitData, bIsGuarding);

	FVector start = GetActorLocation();
	FVector target = DamageData.Attacker->GetActorLocation();
	FVector direction = target - start;
	direction.Normalize();

	// 기본 런치 값 
	float launchStrength = InHitData.Launch;

	if (bIsGuarding)
	{
		launchStrength *= 0.5f;
	}

	LaunchCharacter(-direction * launchStrength, false, false);

	FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(start, target);
	targetRotator.Pitch = 0;
	SetActorRotation(targetRotator);
}

void ACPlayer::Damaged()
{
	// 플레이어가 대쉬 혹은 Evade 일대 
	if (!!State)
	{
		FString v = State->IsDashMode() ? TEXT("True") : TEXT("False");
		//FLog::Print("My Dash State => " + v);
		if (State->IsDashMode() || State->IsEvadeMode())
		{
			// 회피 이펙트 
			PlayEvadeEffetc();
			return;
		}
	}

	//Appyle Damage
	{
		if (!!HealthPoint)
			HealthPoint->Damage(DamageData.Power);

		DamageData.Power = 0.0f;
	}

	// DamageData 
	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* hitData = DamageData.Event->HitData;

		Play_DamageMontage(*hitData);

		hitData->PlaySoundWave(this);
		hitData->PlayEffect(this);
		
		//hitData->PlayHitStop(this);
		//hitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());


		if (HealthPoint->IsDead() == false)
		{
			Launch(*hitData);
		}

		if (!!Condition)
		{
			if (GetCharacterMovement()->IsFalling())
				Condition->AddAirborneCondition();
			if (hitData->bDown)
				Condition->AddDownCondition();
		}
	}

	if (HealthPoint->IsDead())
	{
		State->SetDeadMode();

		return;
	}

	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;
}

void ACPlayer::Play_DamageMontage(const FHitData& hitData)
{
	Super::Play_DamageMontage(hitData);


	UAnimMontage* montage = hitData.Montage;
	float playRate = hitData.PlayRate;

	if (montage == nullptr)
	{

		bool check = false;

		// 공중 상태일 경우 
		bool isAirborne = true;
		isAirborne &= Airborne != nullptr && Airborne->GetIsAirborne() == true;

		// 지상에 다운된 상태인  경우  ( 공중 X 땅 O)
		bool isDowned = true;
		isDowned &= Condition != nullptr && Condition->GetDownCondition() == true;

		// 다운 상태도 아니고 공중 상태도 아니면 지상에 서있는 상태
		check = isAirborne == false && isDowned == false;
		if (check == true)
		{
			// 그러면 다운 시작 애니메이션 재생
			if (hitData.bDown)
				montage = DownBeginMontage;
			else
			{
				montage = DamagedMontage;
				playRate = 1.5f;
			}
		}
		else if (isAirborne == true)
		{
			// 공중 상태에서 맞는 애니메이션 처리
			montage = AirborneDamagedMontage;
			playRate = 1.5f;
		}
		else if (isDowned == true)
		{
			montage = DownDamgeMontage;
		}
	}

	PlayAnimMontage(montage, playRate);

}

void ACPlayer::End_Damaged()
{
	State->SetIdleMode();

	Tags.Remove(FName("HitByWeapon"));
}

void ACPlayer::End_Downed()
{
	Movement->Move();

	if (OnCharacterRaised.IsBound())
		OnCharacterRaised.Broadcast();
}

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	//CheckTrue(InputComponent->GetAxisValue("MoveForward") >= 0.0f);

	State->SetEvadeMode();
}

void ACPlayer::Backstep()
{
	Movement->EnableControlRotation();

	PlayAnimMontage(BackstepMontage);
}

void ACPlayer::End_Backstep()
{
	Movement->DisableControlRotation();

	State->SetIdleMode();
}

void ACPlayer::OnSubAction()
{
	CheckNull(Weapon);

	if (Weapon->IsUnarmedMode())
	{
		CheckFalse(State->IsIdleMode());

		//

		return;
	}

	Weapon->SubAction_Pressed();
}

void ACPlayer::OffSubAction()
{
	CheckNull(Weapon);
	CheckTrue(Weapon->IsUnarmedMode());

	Weapon->SubAction_Released();
}

void ACPlayer::OnJumpAction()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());

	if (Grapple->GetGrappling())
	{
		InterruptGrapple();

		return;
	}

	JumpMaxCount = 2;

	if (JumpCurrentCount < JumpMaxCount)
		Jump();
	else
		OnGrapple();
}

void ACPlayer::OnJumpActionEnd()
{
	StopJumping();
}

void ACPlayer::Jump()
{
	Super::Jump();

	//Movement->EnableControlRotation();
	//FLog::Print("Jump!");

	PlayAnimMontage(JumpMontage);
}

void ACPlayer::OnGrapple()
{
	CheckNull(Grapple);

	//FLog::Print("Grapple!!");
	Grapple->OnGrapple();
}

void ACPlayer::InterruptGrapple()
{
	CheckNull(Grapple);

	if (Grapple->GetGrappling())
	{
		Grapple->InterruptGrapple();
	}
}

void ACPlayer::OnDash()
{

}

void ACPlayer::PlayEvadeEffetc()
{
	FVector effectLocation = GetActorLocation();
	FRotator effectRotation = GetActorRotation();

	// 이펙트 재생
	/*if (EvadeEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EvadeEffect, effectLocation, effectRotation);*/


		// 사운드 재생 (옵션)
		/*if (EvadeSound)
			UGameplayStatics::PlaySoundAtLocation(this, EvadeSound, effectLocation);*/

}

void ACPlayer::OnSkill1()
{
	CheckNull(Weapon);
	CheckFalse(State->IsIdleMode());

	Weapon->ExecuteSkill(0);

}

void ACPlayer::ReleaseSkill1()
{
	CheckNull(Weapon);
	
	Weapon->ReleaseSkill(0);
}

void ACPlayer::OnSkill2()
{
	CheckNull(Weapon);
	CheckFalse(State->IsIdleMode());

	Weapon->ExecuteSkill(1);
}

void ACPlayer::ReleaseSkill2()
{
	CheckNull(Weapon);

	Weapon->ReleaseSkill(1);
}

void ACPlayer::OnSkill3()
{
	CheckNull(Weapon);
	Weapon->ExecuteSkill(2);
}

void ACPlayer::ReleaseSkill3()
{
	CheckNull(Weapon);

	Weapon->ReleaseSkill(2);
}

void ACPlayer::OnSkill4()
{

}

void ACPlayer::ReleaseSkill4()
{
	CheckNull(Weapon);

	Weapon->ReleaseSkill(3);
}



// 캐릭터가 착지되었을 때 콜되는 함수 
void ACPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	CheckFalse(State->IsIdleMode());

	Parkour->DoParkour(true);

	if (bShouldCountDownOnLand)
		StartDownTimer();

	if (!!Grapple)
		Grapple->ResetGrapple();

	FRotator ResetRotation = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	SetActorRotation(ResetRotation);
}

void ACPlayer::FindComponent()
{
	if (Parkour == nullptr)
		Parkour = FHelpers::GetComponent<UCParkourComponent>(this);

	if (Zoom == nullptr)
		Zoom = FHelpers::GetComponent<UCZoomComponent>(this);
}


/////////////////////////////////////////////////////////////////////////////////////////

void ACPlayer::OnIncreaseTimeScale()
{
	FLog::Log("Increase Time Scale");
	AdjustTimeScale(+0.1f);
}

void ACPlayer::OnDecreaseTimeScale()
{
	FLog::Log("Decrease Time Scale");
	AdjustTimeScale(-0.1f);
}

void ACPlayer::AdjustTimeScale(float InTimeScaleData)
{
	float newTimeScale = UGameplayStatics::GetGlobalTimeDilation(GetWorld()) + InTimeScaleData;

	newTimeScale = FMath::Clamp(newTimeScale, 0.1f, 3.0f);

	FLog::Print("TimeScale : " + FString::SanitizeFloat(newTimeScale), 1, 10.0f,
		FColor::Orange);

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), newTimeScale);
}


////////////////////////////////////////////////////////////////////////////////////
void ACPlayer::OnAirborneConditionActivated()
{
	bShouldCountDownOnLand = true;
}

void ACPlayer::OnAirborneConditionDeactivated()
{
}


void ACPlayer::StartDownTimer()
{
	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (capsule)
	{
		//capsule->SetCapsuleHalfHeight(40.0f); // 크기 축소
		//capsule->SetCapsuleRadius(20.0f); // 반지름 축소
	}

	if (!!State)
		State->SetDownMode();

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]()
		{
			if (!!Condition)
				Condition->RemoveDownCondition();
		});

	GetWorld()->GetTimerManager().SetTimer(ChangeConditionHandle, timerDelegate, 5.0f, false);
}

void ACPlayer::OnDownConditionActivated()
{
	CheckNull(Condition);
	CheckTrue(HealthPoint->IsDead());


	// 공중 상태라면 다운 상태에 관한 로직을 바로 하지 않고 델리게이트에 맞겨놓는다.
	if (Condition->GetAirborneCondition())
	{
		bShouldCountDownOnLand = true;
		FLog::Log("Target has Airborne : " + GetName());

		return;
	}


	Movement->Stop();

	StartDownTimer();

	if (OnCharacterDowned.IsBound())
		OnCharacterDowned.Broadcast();
}

void ACPlayer::OnDownConditionDeactivated()
{
	// 다운 상태 풀어볼려했는데 그럴만한 여건이 안되면? 수행안함
	if (GetCharacterMovement()->IsFalling())
	{
		// 착지될 때 시점에 맡긴ㄷㅏ.

		return;
	}

	UCapsuleComponent* capsule = GetCapsuleComponent();
	if (capsule)
	{
		//capsule->SetCapsuleHalfHeight(88.0f); // 기본 크기로 복구
		//capsule->SetCapsuleRadius(34.0f); // 기본 반지름으로 복구
	}

	// 일어나는 애님 진행 - 이 애니메이션에서 상태 바꿈 
	bShouldCountDownOnLand = false;
	if (RaiseMontage != nullptr)
	{
		PlayAnimMontage(RaiseMontage);

		return;
	}

	End_Downed();
}

bool ACPlayer::HasGuard() const
{
	return true;
}

bool ACPlayer::CanGuard() const
{
	CheckNullResult(Guard, false);

	return Guard->GetCanGuard();
}

bool ACPlayer::GetGuarding() const
{
	CheckNullResult(Guard, false);

	return Guard->GetGuarding();
}

void ACPlayer::StartGuard()
{
	// 가드 상태를 할 수 없다면 호출해도 의미 없게 
	CheckNull(Guard);
	CheckFalse(Guard->GetCanGuard());
}

void ACPlayer::StopGuard()
{
	CheckNull(Guard);
}


