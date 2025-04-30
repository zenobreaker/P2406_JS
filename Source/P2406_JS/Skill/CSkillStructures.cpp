#include "Skill/CSkillStructures.h"
#include "Global.h"
#include "Gameframework/Character.h"

#include "Weapons/CWeaponStructures.h" 

#include "Skill/CSkillCollisionComponent.h"
#include "Skill/Entities/CSkillEntity_Attachment.h"


//-----------------------------------------------------------------------------

////TODO :: ������ ��� ������ �� �����ϴ°� �߳� ����Ʈ�� ���� ������ ���� ���ɱ׷��� ���� 
//
//ACSkillEntity* FSkillPhaseData::ExecutePhase(ACharacter* InCharacter, FName InSectionName)
//{
//	Phase_DoAction(InCharacter, InSectionName);
//	Phase_PlaySoundWave(InCharacter);
//	Phase_PlayEffect(InCharacter);
//	Phase_PlayCameraShake(InCharacter);
//	return Phase_SpawnSkillEntity(InCharacter);
//}

void FSkillPhaseData::ExecutePhase(ACharacter* InCharacter, FName InSectionName)
{
	Phase_DoAction(InCharacter, InSectionName);
	Phase_PlaySoundWave(InCharacter);
	Phase_PlayEffect(InCharacter);
	Phase_PlayCameraShake(InCharacter);
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
	rotator += EffectRotation;

	location += rotator.RotateVector(EffectLocation);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetRotation(FQuat(rotator));
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

//-----------------------------------------------------------------------------

ACSkillEntity* FSkillPhaseData::Phase_SpawnSkillEntity(ACharacter* InCharacter)
{
	CheckNullResult(InCharacter, nullptr);

	return SkillEntityData.SpawnSkillEntity(InCharacter, Effect);
}


//----------------------------------------------------------------------------------


FSkillEntityData::FSkillEntityData()
{
	SkillEntity = ACSkillEntity::StaticClass();

	SpawnLocation = FVector::ZeroVector;
	SpawnRotation = FRotator::ZeroRotator;
	SpawnScale = FVector(1.0f, 1.0f, 1.0f);

	SkillCollisionDatas = TArray<FSkillCollisionData>();
}


ACSkillEntity* FSkillEntityData::SpawnSkillEntity(ACharacter* InCharacter, UFXSystemAsset* InEffect)
{
	CheckNullResult(InCharacter, nullptr);
	CheckNullResult(SkillEntity, nullptr);

	// ���� ���� ���� 
	FActorSpawnParameters param;
	param.Owner = InCharacter;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	// ��ų ���� ��ġ ���� 
	FVector spawnLocation = InCharacter->GetActorLocation()
		+ InCharacter->GetActorForwardVector() * SpawnLocation.X
		+ InCharacter->GetActorRightVector() * SpawnLocation.Y
		+ InCharacter->GetActorUpVector() * SpawnLocation.Z;

	FRotator spawnRotation = InCharacter->GetActorRotation();

	FTransform transform;
	transform.SetRotation(FQuat(spawnRotation));
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
	SetSkillCollisionWithEntity(InCharacter, skillEntity);
	UGameplayStatics::FinishSpawningActor(skillEntity, transform);

	return skillEntity;
}

void FSkillEntityData::SetSkillCollisionWithEntity(ACharacter* InCharacter, ACSkillEntity* InEntity)
{
	CheckNull(InCharacter);
	CheckNull(InEntity);

	InEntity->SetOwnerCharacter(InCharacter);	
	InEntity->SetSkillEntityData(SkillCollisionDatas);
}

//----------------------------------------------------------------------------------

FSkillCollisionData::FSkillCollisionData()
{
	SkillCollisionClass = UCSkillCollisionComponent::StaticClass();
}

UFXSystemAsset* FSkillCollisionData::GetSkillEffectAsset()
{
	return SkillEffect.GetEffect();
}

void FSkillCollisionData::Play_SkillEffect(AActor* InActor)
{
	SkillEffect.Play_Effect(InActor);
}

UFXSystemAsset* FSkillEffectInfo::GetEffect()
{
	return SkillEffectAsset;
}

void FSkillEffectInfo::Play_Effect(AActor* InActor)
{
	CheckNull(InActor);
	CheckNull(SkillEffectAsset);

	FVector location = InActor->GetActorLocation();
	FRotator rotator = InActor->GetActorRotation();
	rotator += EffectRotation;

	location += rotator.RotateVector(EffectLocation);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetRotation(FQuat(rotator));
	transform.SetScale3D(EffectScale);

	FHelpers::PlayEffect(InActor->GetWorld(), SkillEffectAsset, transform);
}
