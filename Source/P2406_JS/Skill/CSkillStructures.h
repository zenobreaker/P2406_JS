#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "Enums/FEnum.h"
#include "Components/CWeaponComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "CSkillStructures.generated.h"


USTRUCT(BlueprintType)
struct FSkillInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	EWeaponType WeaponType = EWeaponType::Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	int32 SkillID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	UTexture2D* SkillIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	FString SkillName = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float CastingTime = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float CoolDown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Info")
	float Cost = 0.0f;
};


/// <summary>
/// 스킬 충돌체  데미지 등 관련
/// </summary>
USTRUCT(BlueprintType)
struct FSkillCollisionData 
{
	GENERATED_BODY()

public:	
	UPROPERTY(EditAnywhere, Category = "Interval")
	bool bRepeat = false; 

	UPROPERTY(EditAnywhere, Category = "Interval")
	float CollisionInterval = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Collision")
	TSubclassOf<class UCSkillCollisionComponent> SkillCollisionClass;

	UPROPERTY(EditAnywhere, Category = "Collision")
	ESkillCollisionType CollisionType =  ESkillCollisionType::Sphere;

	UPROPERTY(EditAnywhere, Category = "Collision")
	FVector BoxExtent  = FVector::OneVector;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float CapsuleRadius = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Collision")
	float CapsuleHalfHeight = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Hit Data")
	TArray<FHitData> HitDatas;

public:
	FSkillCollisionData();
};

/// <summary>
/// 스킬 충돌체 관리자(Skill Entity) 생성 정보 구조체
/// </summary>
USTRUCT(BlueprintType)
struct FSkillEntityData
{
    GENERATED_BODY()

public:
	// 생성할 클래스
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACSkillEntity> SkillEntity;

    /** 스폰할 위치 (기본값: 캐릭터 위치) */
    UPROPERTY(EditAnywhere)
    FVector SpawnLocation = FVector::ZeroVector;

    /** 방향 (Projectile 이나 Melee 용) */
    UPROPERTY(EditAnywhere)
    FRotator SpawnRotation = FRotator::ZeroRotator;

    /** 스케일 (Collision 의 크기 조정) */
    UPROPERTY(EditAnywhere)
    FVector SpawnScale = FVector(1.0f, 1.0f, 1.0f);

    /** 오프셋 (캐릭터 위치 기준 상대 위치) */
    UPROPERTY(EditAnywhere)
    FVector Offset = FVector::ZeroVector;

	// 이 콜리전의 판정 정보
	UPROPERTY(EditAnywhere)
	FSkillCollisionData  SkillCollisionData;

public:
	FSkillEntityData();

	ACSkillEntity* SpawnSkillEntity(class ACharacter* InCharacter);
};

/// <summary>
/// 각 스킬 페이즈별 실행할 정보 구조체  
/// </summary>
USTRUCT(BlueprintType)
struct FSkillPhaseData 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Phase")
	ESkillPhase PhaseType = ESkillPhase::Max;

	UPROPERTY(EditAnywhere, Category = "Phase")
	FDoActionData ActionData;

	UPROPERTY(EditAnywhere, Category = "Phase")
	class UFXSystemAsset* Effect = nullptr;

	UPROPERTY(EditAnywhere, Category = "Phase")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Phase")
	FVector EffectScale = FVector(1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Phase")
	class USoundWave* Sound = nullptr;

	UPROPERTY(EditAnywhere, Category = "Phase")
	TSubclassOf<class UCameraShakeBase> CameraShake; 

	// 충돌판정 스킬 딜레이 값 이 값이 지나야 충돌판정이 생성되게함
	UPROPERTY(EditAnywhere, Category = "Skill Collsion")
	float CollisionCreateDelay = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Skill Entity")
	FSkillEntityData SkillEntityData;


public:
	FSkillPhaseData() = default; 

public:
	ACSkillEntity* ExecutePhase(class ACharacter* InCharacter, FName InSectionName = NAME_None);
	void Phase_AnimationPlayback(class ACharacter* InCharacter, float InValue);
	void Phase_DoAction(class ACharacter* InCharacter, FName InSectionName = NAME_None);
	void Phase_PlaySoundWave(class ACharacter* InCharacter);
	void Phase_PlayEffect(class ACharacter* InCharacter);
	void Phase_PlayCameraShake(class ACharacter* InCharacter);
	ACSkillEntity* Phase_SpawnSkillEntity(class ACharacter* InCharacter);
};

///<summary>
/// 각 스킬 페이즈별 실행할 정보 구조체 
///</summary>
USTRUCT(BlueprintType)
struct FSkillFlowData 
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<FSkillPhaseData> PhaseDatas;
};


UCLASS()
class P2406_JS_API UCSkillStructures : public UObject
{
	GENERATED_BODY()
};
