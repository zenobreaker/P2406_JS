#include "Skill/CSkillStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponStructures.h" 
#include "Skill/CSkillCollision.h"
#include "Gameframework/Character.h"

void FSkillActionData::DoAction(ACharacter* InOwner)
{
	Super::DoAction(InOwner);

	// �ִϸ��̼��� ������ ����
	InOwner->PlayAnimMontage(Montage);
	// ����Ʈ�� �ִٸ� �װ͵� ����
}

void FSkillActionData::Create_SkillCollision(ACharacter* InOwner, const TArray<FSkillHitData>& InHitDatas)
{
	CheckNull(SkillCollisionClass);
	CheckNull(InOwner);

	// ���� ���� ���� 
	FActorSpawnParameters param;
	param.Owner = InOwner;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// ��ų ���� ��ġ ���� 
	FTransform transform;
	transform.SetLocation(InOwner->GetActorLocation());

	ACSkillCollision* skillCollision = InOwner->GetWorld()->SpawnActorDeferred<ACSkillCollision>(SkillCollisionClass,
		transform,
		nullptr,
		nullptr,
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	CLog::Print("Success Create!");

	// �ݸ����� ������ ����
	CheckNull(skillCollision);
	skillCollision->SetSkillOwnerData(InOwner, InHitDatas);
	skillCollision->AddIgnore(InOwner);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);

	// �� �ڵ�� �� �Ʒ� �ڵ�� ���� ����
	//skillCollision->SetActorTransform(transform); // ��ġ�� ��ȯ�� ����
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

void FSkillActionData::PlaySection_SkillCastingMontage(ACharacter* InOwner, float InPlayRate,
	FName StartSectionName)
{
	CheckNull(InOwner);

	// ��¥ ��.. �� �����Ÿ��� �س������� �� �ʺ��� ������ 
	//TODO: ������ �ִϸ��̼� �ϳ��� ���յǰ� �ع�����??
	InOwner->PlayAnimMontage(BeginCastingAnimMontage, InPlayRate, StartSectionName);
}

void FSkillActionData::PlaySecion_SkillActionMontage(ACharacter* InOwner, float InPlayRate, FName StartSectionName)
{
	CheckNull(InOwner);

	//TODO: ������ �ִϸ��̼� �ϳ��� ���յǰ� �ع�����??
	InOwner->PlayAnimMontage(Montage, InPlayRate, StartSectionName);
}

void FSkillActionData::Begin_Casting(ACharacter* InOwner, bool InLoop)
{
	SkillPlayMontage(InOwner, BeginCastingAnimMontage, InLoop);
}

void FSkillActionData::DoCasting(ACharacter* InOwner, bool InLoop)
{
	SkillPlayMontage(InOwner, CastingAnimMontage, InLoop);
}

void FSkillActionData::End_Casting(ACharacter* InOwner, bool InLoop)
{
	SkillPlayMontage(InOwner, EndCastingAnimMontage, InLoop);
}

void FSkillActionData::PlaySoundWave(ACharacter* InOwner)
{
	CheckNull(Sound);

	UWorld* world = InOwner->GetWorld();
	FVector location = InOwner->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FSkillActionData::Destroy_GhostTrail()
{
	Super::Destroy_GhostTrail();
}

void FSkillActionData::SkillPlayMontage(ACharacter* InOwner,
	UAnimMontage* InMontage, bool InLoop)
{
	CheckNull(InOwner);
	CheckNull(InMontage);

	InOwner->PlayAnimMontage(InMontage);

	if (InLoop)
	{
		UAnimInstance* animInstance = InOwner->GetMesh()->GetAnimInstance();
		CheckNull(animInstance);
		//animInstance->OnMontageEnded.AddDynamic(this, )
	}

}

