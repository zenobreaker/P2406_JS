#include "Characters/CPlayer.h"
#include "Global.h"
#include "CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CMovementComponent.h"


#include "Weapons/CSword.h"
#include "Animation/AnimMontage.h"

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

} 

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun();
	Movement->DisableControlRotation();

	FActorSpawnParameters params;
	params.Owner = this;

	Sword = GetWorld()->SpawnActor<ACSword>(ACSword::StaticClass(), params);
	Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Holster_Sword");

	 
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

	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, Weapon, &UCWeaponComponent::SetSwordMode);
}

void ACPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	/*switch (InNewType)
	{
		case EStateType::Evade: Backstep(); break;
	}*/
}

void ACPlayer::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{

	bool bVisible = false;

	if (InNewType == EWeaponType::Bow)
		bVisible = true;

}

void ACPlayer::Equip()
{
	CheckTrue(bEquipping);

	bEquipping = true; 

	if (!!EquipMontage)
		PlayAnimMontage(EquipMontage, 2);
}

