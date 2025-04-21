#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/CSkillStructures.h"
#include "Enums/FEnum.h"
#include "Weapons/AddOns/AttackInterface.h"
#include "CSkillEntity.generated.h"



/// 맵의 배치되는 스킬 오브젝트 
/// 스킬의 속성, 충돌관리하는 CollisionComponent를 관리
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
	int32 GetDamagedCount();

	virtual void SetSkillEntityData(TArray<FSkillCollisionData>& InDatas);

public:
	UFUNCTION()
	void DestroySkill();

	UFUNCTION()
	void OnDamaged(class AActor* InOther);

private:
	virtual void ActivateCollisionSequence(FName InName = "Default");
	virtual void ActivateCollisionComponent(class UCSkillCollisionComponent* InSCC);

public:
	// 충돌 시작
	virtual void ActivateCollision(FName InName = "Default");
	// 충돌 종료
	virtual void DeactivateCollision(FName InName = "Default");
	virtual void UpdateSCC(FName InName, class UCSkillCollisionComponent* InSCC) {}
	
public:
	// IAttackInterface을(를) 통해 상속됨
	AActor* GetDamageSource() const override;

protected:
	class ACharacter* OwnerCharacter;

protected:
	float CollisionInterval = -1.0f;
	int32 Index;

protected:
	TMap<FName, TArray<class UCSkillCollisionComponent*>> CollisionTable;

private:
	TArray<AActor*> DamagedList;
	FTimerHandle TimerHandle;
};
