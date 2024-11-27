#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CDashComponent.h"
#include "Components/CSkillComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Characters/CGhostTrail.h"
#include "Widgets/CUserWidget_Player.h"
#include "Weapons/CWeaponStructures.h"

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCDashComponent>(this, &Dash, "Dash");
	CHelpers::CreateActorComponent<UCTargetComponent>(this, &Target, "Target");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCParkourComponent>(this, &Parkour, "Parkour");
	CHelpers::CreateActorComponent<UCGrapplingComponent>(this, &Grapple, "Grapple");
	CHelpers::CreateActorComponent<UCHealthPointComponent>(this, &HealthPoint, "Health");
	CHelpers::CreateActorComponent<UCSkillComponent>(this, &Skill, "Skill");

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Mesh/SK_Mannequin.SK_Mannequin'");
	GetMesh()->SetSkeletalMesh(mesh);

	TSubclassOf<UCAnimInstance> animInstance;
	CHelpers::GetClass<UCAnimInstance>(&animInstance, "/Script/Engine.AnimBlueprint'/Game/ABP_CPlayer.ABP_CPlayer_C'");
	GetMesh()->SetAnimClass(animInstance);

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	// 애로우 컴포넌트 가져오기 
	CHelpers::CreateComponent<USceneComponent>(this, &ArrowGroup, "Arrows", GetCapsuleComponent());
	for (int32 i = 0; i < (int32)EParkourArrowType::Max; i++)
	{
		FString name = StaticEnum<EParkourArrowType>()->GetNameStringByIndex(i);
		CHelpers::CreateComponent<UArrowComponent>(this, &Arrows[i], FName(name),
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


	CHelpers::GetAsset<UAnimMontage>(&BackstepMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/BackStep_Montage.BackStep_Montage'");

	CHelpers::GetAsset<UAnimMontage>(&JumpMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/Unarmed_JumpStart_Montage.Unarmed_JumpStart_Montage'");

	if (UiClass == nullptr)
		CHelpers::GetClass<UCUserWidget_Player>(&UiClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");

	if (!!Grapple)
	{
		Grapple->PrimaryComponentTick.bCanEverTick = true;
	}


}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun();
	Movement->DisableControlRotation();


	State->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChanged);
	Weapon->OnWeaponTypeChanged.AddDynamic(this, &ACPlayer::OnWeaponTypeChanged);


	if (!!UiClass)
	{
		UserInterface = Cast<UCUserWidget_Player>(CreateWidget(GetController<APlayerController>(), UiClass));
		if (!!UserInterface)
		{
			UserInterface->AddToViewport();
			//UserInterface->UpdateWeaponType(EWeaponType::Max);
			UserInterface->UpdateCrossHairVisibility(false);
		}
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

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSubAction);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffSubAction);

	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Pressed, this, &ACPlayer::OnJumpAction);
	PlayerInputComponent->BindAction("Jumping", EInputEvent::IE_Released, this, &ACPlayer::OnJumpActionEnd);

	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);

	PlayerInputComponent->BindAction("Target", EInputEvent::IE_Pressed, Target, &UCTargetComponent::Toggle);
	PlayerInputComponent->BindAction("Target_Left", EInputEvent::IE_Pressed, Target, &UCTargetComponent::MoveLeft);
	PlayerInputComponent->BindAction("Target_Right", EInputEvent::IE_Pressed, Target, &UCTargetComponent::MoveRight);

	//TODO: 일단무식한 방법으로 처리함 나중에 개념 충족하면 여기 수정하기 
	PlayerInputComponent->BindAction("Skill1", EInputEvent::IE_Pressed, this,
		&ACPlayer::OnSkill1);

	PlayerInputComponent->BindAction("Skill2", EInputEvent::IE_Pressed, this,
		&ACPlayer::OnSkill2);

}

float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	DamageData.Power = Damage;
	DamageData.Attacker = Cast<ACharacter>(EventInstigator->GetPawn());
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	State->SetDamagedMode();

	return Damage;
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Evade: Backstep(); break;
		case EStateType::Damaged: Damaged(); break;
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
		CLog::Print("UserInteface !");
		UserInterface->UpdateCrossHairVisibility(bVisible);
	}
	else
	{
		CLog::Print("UserInteface NO");
	}
}

void ACPlayer::Damaged()
{
	// 플레이어가 대쉬 혹은 Evade 일대 
	if (!!State)
	{
		FString v = State->IsDashMode() ?  TEXT("True") : TEXT("False");
		CLog::Print("My Dash State => " + v);
		if (State->IsDashMode() || State->IsEvadeMode())
		{
			// 회피 이펙트 
			PlayEvadeEffetc();
			return; 
		}
	}

	//Appyl Damage
	{
		if (!!HealthPoint)
			HealthPoint->Damage(DamageData.Power);

		DamageData.Power = 0.0f;
	}

	// DamageData 
	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* hitData = DamageData.Event->HitData;


		UAnimMontage* montage = hitData->Montage;
		float playRate = hitData->PlayRate;

		if (montage == nullptr)
		{
			montage = DamagedMontage;
			playRate = 1.5f;
		}
		PlayAnimMontage(montage, playRate);


		hitData->PlayHitStop(GetWorld());
		hitData->PlaySoundWave(this);
		hitData->PlayEffect(GetWorld(), GetActorLocation(), GetActorRotation());

		if (HealthPoint->IsDead() == false)
		{
			FVector start = GetActorLocation();
			FVector target = DamageData.Attacker->GetActorLocation();
			FVector direction = target - start;
			direction.Normalize();

			LaunchCharacter(-direction * hitData->Launch, false, false);
			FRotator targetRotator = UKismetMathLibrary::FindLookAtRotation(start, target);
			targetRotator.Pitch = 0;
			SetActorRotation(targetRotator);

			// 공중에 띄우기
			/*if(!!Airborne)
				Airborne->LaunchIntoAir(hitData->Airial, DamageData.Attacker);*/
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

void ACPlayer::End_Damaged()
{
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
	CLog::Print("Jump!");

	PlayAnimMontage(JumpMontage);
}

void ACPlayer::OnGrapple()
{
	CheckNull(Grapple);
	

	CLog::Print("Grapple!!");
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

void ACPlayer::OnSkill2()
{
	CheckNull(Weapon);
	CheckFalse(State->IsIdleMode());

	Weapon->ExecuteSkill(1);
}



// 캐릭터가 착지되었을 때 콜되는 함수 
void ACPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	
	CheckFalse(State->IsIdleMode());

	Parkour->DoParkour(true);

	CheckNull(Grapple);
	Grapple->ResetGrapple();

	FRotator ResetRotation = FRotator(0.0f, GetActorRotation().Yaw, 0.0f);
	SetActorRotation(ResetRotation);
}

