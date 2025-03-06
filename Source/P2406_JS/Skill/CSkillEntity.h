#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/CSkillStructures.h"
#include "Enums/FEnum.h"
#include "Weapons/AddOns/AttackInterface.h"
#include "CSkillEntity.generated.h"



/// <summary>
/// 맵의 배치되는 스킬 오브젝트 
/// 스킬의 속성, 충돌관리하는 CollisionComponent를 관리
/// </summary>
UCLASS()
class P2406_JS_API ACSkillEntity 
	: public AActor
	, public IAttackInterface
{
	GENERATED_BODY()


public:
	FORCEINLINE void SetOwnerCharacter(class ACharacter* InCharacter) { OwnerCharacter = InCharacter; }


public:	
	ACSkillEntity();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	//FORCEINLINE void SetSkillCollisionType(ESkillCollisionType InType) { MyType = InType; }
	int32 GetDamagedCount();
	void SetSkillEntityData(FSkillCollisionData InData);

public:
	UFUNCTION()
	void DestroySkill(); 

protected:
	void CreateCollisionByType(FSkillCollisionData InData);

public:
	// 충돌 시작
	virtual void ActivateCollision();
	virtual void ActivateCollision(FName InName);
	// 충돌 종료
	virtual void DeactivateCollision();
	virtual void DeactivateCollision(FName InName);

	void SetSkillDamageEvent(TArray<TFunction<void()>> InFuncs);
	void SetSkillDamageEventOneParam(TArray<TFunction<void(ACharacter*)>> InFuncs);
	void SetSkillDamageEventThreeParams(TArray<TFunction<void(ACharacter*, AActor*, ACharacter*)>> InFuncs);


public:
	// IAttackInterface을(를) 통해 상속됨
	AActor* GetDamageSource() const override;

private:
	TArray<AActor*> Hitted;

protected:
	class ACharacter* OwnerCharacter;

protected:
	class UCSkillCollisionComponent* SkillCollision;
	//TArray<class UCSkillCollisionComponent*> SkillCollisions;
	TMap<FName, class UCSkillCollisionComponent*> CollisionTable;
};
