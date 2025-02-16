#include "Skill/SkillExecuter.h"
#include "Global.h"
#include "Gameframework/Character.h"

void SkillExecuter::Phase_DoAction(ACharacter* InCharacter, FDoActionData& ActionData)
{
	ActionData.DoAction(InCharacter);
}

void SkillExecuter::Phase_PlayEffect(ACharacter* InCharacter, UFXSystemAsset* Effect, FVector Location, FVector Scale)
{
	CheckNull(InCharacter);
	CheckNull(Effect);

	FVector location = InCharacter->GetActorLocation();
	FRotator rotator = InCharacter->GetActorRotation();

	location += rotator.RotateVector(Location);

	FTransform transform;
	transform.SetLocation(location);
	transform.SetScale3D(Scale);

	FHelpers::PlayEffect(InCharacter->GetWorld(), Effect, transform);
}

void SkillExecuter::Phase_PlaySound(ACharacter* InCharacter, USoundWave* Sound)
{
	CheckNull(Sound);

	UWorld* world = InCharacter->GetWorld();
	FVector location = InCharacter->GetActorLocation();

	UGameplayStatics::SpawnSoundAtLocation(world, Sound, location);
}

void SkillExecuter::Phase_PlayCameraShake(ACharacter* InCharacter, TSubclassOf<UCameraShakeBase> CameraShake)
{
	CheckNull(InCharacter);
	CheckNull(CameraShake);

	APlayerCameraManager* cameraManager = UGameplayStatics::GetPlayerCameraManager(InCharacter->GetWorld(), 0);
	CheckNull(cameraManager);

	cameraManager->StartCameraShake(CameraShake);
}

void SkillExecuter::Phase_SpawnSkillEntity(ACharacter* InCharacter, FSkillEntityData& SkillEntityData)
{
	SkillEntityData.SpawnSkillEntity(InCharacter);
}
