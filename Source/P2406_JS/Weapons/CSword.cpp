#include "Weapons/CSword.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

ACSword::ACSword()
{
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &Mesh, "Mesh", Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Capsule, "Capsule", Mesh);

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, "/Script/Engine.SkeletalMesh'/Game/Characters/Weapons/Sword/SK_Sword.SK_Sword'");
	Mesh->SetSkeletalMesh(mesh);

	Capsule->SetRelativeLocation(FVector(-12, -130, 0)); 
	Capsule->SetRelativeRotation(FRotator(0, 0, 90));
	Capsule->SetCapsuleHalfHeight(220);
	Capsule->SetCapsuleRadius(11);
}

void ACSword::BeginPlay()
{
	Super::BeginPlay();
	
	Capsule->OnComponentBeginOverlap.AddDynamic(this, &ACSword::OnBeginOverlap);
}

void ACSword::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//FDamageEvent e;

	//OtherActor->TakeDamage(20, e, GetOwner()->GetInstigatorController(), this);
}

