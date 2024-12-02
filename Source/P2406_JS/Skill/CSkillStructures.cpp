#include "Skill/CSkillStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponStructures.h" 
#include "Skill/CSkillCollision.h"
#include "Gameframework/Character.h"

void FSkillActionData::DoAction(ACharacter* InOwner)
{
	Super::DoAction(InOwner);

	// 애니메이션이 있으면 실행
	InOwner->PlayAnimMontage(Montage);
	// 이펙트가 있다면 그것도 실행
}

void FSkillActionData::Create_SkillCollision(ACharacter* InOwner, const TArray<FSkillHitData>& InHitDatas)
{
	CheckNull(SkillCollisionClass);

	FActorSpawnParameters param;
	param.Owner = InOwner;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FTransform transform;
	ACSkillCollision* skillCollision = InOwner->GetWorld()->SpawnActorDeferred<ACSkillCollision>(SkillCollisionClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	CheckNull(skillCollision);
	skillCollision->SetSkillOwnerData(InOwner, InHitDatas);

	skillCollision->AddIgnore(InOwner);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);

	UGameplayStatics::FinishSpawningActor(skillCollision, transform);
}

void FSkillActionData::Create_SkillEffect(ACharacter* InOwner)
{
	CheckNull(InOwner);

	FVector ownerLocation = InOwner->GetActorLocation();
	FVector ownerForward = InOwner->GetActorForwardVector();
	FVector spwanLocation = ownerLocation + ownerForward;

	FRotator spawnRotator = InOwner->GetActorRotation();
	//spawnRotator.Roll = angle;

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		InOwner->GetWorld(),
		SkillEffect,
		spwanLocation,
		spawnRotator,
		FVector::OneVector
	);
}

void FSkillActionData::Begin_Casting(ACharacter* InOwner)
{
	CheckNull(InOwner);

	InOwner->PlayAnimMontage(BeginCastingAnimMontage);
}

void FSkillActionData::DoCasting(ACharacter* InOwner)
{
	CheckNull(InOwner);

	InOwner->PlayAnimMontage(CastingAnimMontage);
}

void FSkillActionData::End_Casting(ACharacter* InOwner)
{
	CheckNull(InOwner);

	InOwner->PlayAnimMontage(EndCastingAnimMontage);
}

void FSkillActionData::Destroy_GhostTrail()
{
	Super::Destroy_GhostTrail();
}
