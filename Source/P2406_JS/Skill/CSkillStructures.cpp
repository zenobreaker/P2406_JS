#include "Skill/CSkillStructures.h"
#include "Global.h"
#include "Gameframework/Character.h"

#include "Weapons/CWeaponStructures.h" 
#include "Skill/CSkillEntity.h"
#include "Skill/CSkillCollisionComponent.h"
#include "Skill/Entities/CSkillEntity_Attachment.h"


//-----------------------------------------------------------------------------

//TODO :: ������ ��� ������ �� �����ϴ°� �߳� ����Ʈ�� ���� ������ ���� ���ɱ׷��� ���� 

ACSkillEntity* FSkillPhaseData::ExecutePhase(ACharacter* InCharacter, FName InSectionName)
{
	Phase_DoAction(InCharacter, InSectionName);
	Phase_PlaySoundWave(InCharacter);
	Phase_PlayEffect(InCharacter);
	Phase_PlayCameraShake(InCharacter);
	return Phase_SpawnSkillEntity(InCharacter);
}

void FSkillPhaseData::Phase_AnimationPlayback(ACharacter* InCharacter, float InValue)
{
	ActionData.AnimationPlayback(InCharacter, InValue);
}

void FSkillPhaseData::Phase_DoAction(ACharacter* InCharacter, FName InSectionName)
{
	CheckNull(InCharacter);

	ActionData.DoAction(InCharacter, InSectionName);
}

void FSkillPhaseData::Phase_PlaySoundWave(ACharacter* InCharacter)
{
	CheckNull(Sound);

	UWorld* world = InCharacter->GetWorld();
	FVector location = InCharacter->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void FSkillPhaseData::Phase_PlayEffect(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(Effect);

	FVector location = InCharacter->GetActorLocation();
	FRotator rotator = InCharacter->GetActorRotation();

	location += rotator.RotateVector(EffectLocation);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetScale3D(EffectScale);

	FHelpers::PlayEffect(InCharacter->GetWorld(), Effect, transform);
}

void FSkillPhaseData::Phase_PlayCameraShake(ACharacter* InCharacter)
{
	CheckNull(InCharacter);
	CheckNull(CameraShake);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(InCharacter->GetWorld(), 0);
	CheckNull(cameraManager);

	cameraManager->StartCameraShake(CameraShake);
}

ACSkillEntity* FSkillPhaseData::Phase_SpawnSkillEntity(ACharacter* InCharacter)
{
	CheckNullResult(InCharacter, nullptr);

	return SkillEntityData.SpawnSkillEntity(InCharacter);
}

//-----------------------------------------------------------------------------

ACSkillEntity* FSkillEntityData::SpawnSkillEntity(ACharacter* InCharacter)
{
	CheckNullResult(InCharacter, nullptr);
	CheckNullResult(SkillEntity, nullptr);

	// ���� ���� ���� 
	FActorSpawnParameters param;
	param.Owner = InCharacter;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	// ��ų ���� ��ġ ���� 
	FVector spawnLocation = InCharacter->GetActorLocation()
		+ InCharacter->GetActorForwardVector() * Offset.X
		+ InCharacter->GetActorRightVector() * Offset.Y
		+ InCharacter->GetActorUpVector() * Offset.Z;


	FTransform transform;
	transform.SetLocation(spawnLocation);

	ACSkillEntity* skillEntity = InCharacter->GetWorld()->SpawnActorDeferred<ACSkillEntity>
		(
			SkillEntity,
			transform,
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
		);

	CheckNullResult(skillEntity, nullptr);

	FLog::Log("Entity Create");
	skillEntity->SetOwnerCharacter(InCharacter);
	skillEntity->SetSkillEntityData(SkillCollisionData);
	UGameplayStatics::FinishSpawningActor(skillEntity, transform);

	return skillEntity;
}

FSkillEntityData::FSkillEntityData()
{
	SkillEntity = ACSkillEntity::StaticClass();
}

FSkillCollisionData::FSkillCollisionData()
{
	SkillCollisionClass = UCSkillCollisionComponent::StaticClass();
}
