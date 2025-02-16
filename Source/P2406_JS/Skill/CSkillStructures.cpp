#include "Skill/CSkillStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Weapons/CWeaponStructures.h" 
#include "Skill/CSkillEntity.h"
#include "Gameframework/Character.h"


//-----------------------------------------------------------------------------

ACSkillEntity* FSkillPhaseData::ExecutePhase(ACharacter* InCharacter)
{
	Phase_DoAction(InCharacter);
	Phase_PlaySoundWave(InCharacter);
	Phase_PlayEffect(InCharacter);
	Phase_PlayCameraShake(InCharacter);
	return Phase_SpawnSkillEntity(InCharacter);
}

void FSkillPhaseData::Phase_DoAction(ACharacter* InCharacter)
{
	CheckNull(InCharacter);

	ActionData.DoAction(InCharacter);
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

	// 스폰 형태 설정 
	FActorSpawnParameters param;
	param.Owner = InCharacter;
	param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;



	// 스킬 생성 위치 설정 
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

	FLog::Log("Entity Create");
	if (skillEntity != nullptr)
	{
		skillEntity->SetOwnerCharacter(InCharacter);
		//skillEntity->SetSkillCollisionType(Type);
		skillEntity->SetSkillEntityData(SkillCollisionData);
	}

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepWorld, true);


	// 이 코드는 맨 아래 코드와 같은 역할
	 // 위치와 변환을 설정
	//skillCollision->SetActorTransform(transform); 
	//skillCollision->FinishSpawning(transform);

	UGameplayStatics::FinishSpawningActor(skillEntity, transform);
	return skillEntity;
}
