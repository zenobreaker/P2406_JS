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
	CheckNull(InOwner); 

	// 스폰 형태 설정 
	FActorSpawnParameters param;
	param.Owner = InOwner;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 스킬 생성 위치 설정 
	FTransform transform;
	transform.SetLocation(InOwner->GetActorLocation());

	ACSkillCollision* skillCollision = InOwner->GetWorld()->SpawnActorDeferred<ACSkillCollision>(SkillCollisionClass, 
		transform, 
		nullptr, 
		nullptr, 
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// 콜리전에 데이터 설정
	CheckNull(skillCollision);
	skillCollision->SetSkillOwnerData(InOwner, InHitDatas);
	skillCollision->AddIgnore(InOwner);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);

	// 이 코드는 맨 아래 코드와 같은 역할
	//skillCollision->SetActorTransform(transform); // 위치와 변환을 설정
	//skillCollision->FinishSpawning(transform);
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
