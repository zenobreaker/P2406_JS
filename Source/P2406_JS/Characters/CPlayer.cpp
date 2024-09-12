#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CMovementComponent.h"
#include "Widgets/CUserWidget_Player.h"

ACPlayer::ACPlayer()
{
	CHelpers::CreateComponent(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &Weapon, "Weapon");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	
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


	CHelpers::GetAsset<UAnimMontage>(&BackstepMontage, "/Script/Engine.AnimMontage'/Game/Characters/Montages/BackStep_Montage.BackStep_Montage'");

	if(UiClass==nullptr)
		CHelpers::GetClass<UCUserWidget_Player>(&UiClass, "/Script/UMGEditor.WidgetBlueprint'/Game/Widgets/WB_Player.WB_Player_C'");
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
		UserInterface->AddToViewport();
		//UserInterface->UpdateWeaponType(EWeaponType::Max);
		UserInterface->UpdateCrossHairVisibility(false);
	}
	 
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", Movement, &UCMovementComponent::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", Movement, &UCMovementComponent::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", Movement, &UCMovementComponent::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", Movement, &UCMovementComponent::OnHorizontalLook);


	PlayerInputComponent->BindAction("Sprint",EInputEvent::IE_Pressed, Movement, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, Movement, &UCMovementComponent::OnRun);

	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetFistMode);
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
	PlayerInputComponent->BindAction("Hammer", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetHammerMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetBowMode);

	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::DoAction);

	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Pressed, this, &ACPlayer::OnSubAction);
	PlayerInputComponent->BindAction("SubAction", EInputEvent::IE_Released, this, &ACPlayer::OffSubAction);

	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Evade: Backstep(); break;
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

void ACPlayer::OnEvade()
{
	CheckFalse(State->IsIdleMode());
	CheckFalse(Movement->CanMove());
	
	CheckTrue(InputComponent->GetAxisValue("MoveForward") >= 0.0f);

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

