#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/CSkillStructures.h"
#include "Weapons/AddOns/AttackInterface.h"
#include "CSkillCollisionComponent.generated.h"

DECLARE_DELEGATE(FOnSkillDamaged);
DECLARE_DELEGATE_OneParam(FOnSkillDamagedOneParam, ACharacter*);
DECLARE_DELEGATE_ThreeParams(FOnSkillDamagedThreeParams, ACharacter*, AActor*, ACharacter*);

UCLASS(BlueprintType, Blueprintable)
class P2406_JS_API UCSkillCollisionComponent 
	: public UActorComponent
	, public IAttackInterface
{
	GENERATED_BODY()

public:
	FORCEINLINE void AddIgnore(AActor* InActor) { Ignores.Add(InActor); }
	FORCEINLINE int32 GetDamagedCount() const { return Hitted.Num(); }

protected:
	virtual void BeginPlay() override;

public:
	void SetCollisionData(class ACharacter* InOwner, FSkillCollisionData& InCollisinData);

public:
	// 충돌 시작
	virtual void ActivateCollision() {}
	// 충돌 종료
	virtual void DeactivateCollision() {}
	// 충돌 검사 
	virtual void CheckCollision() {}


public: 
	TArray<FOnSkillDamaged> OnSkillDamageds;
	TArray<FOnSkillDamagedOneParam> OnSkillDamagedOneParams;
	TArray<FOnSkillDamagedThreeParams> OnSkillDamagedThreeParams;

protected:
	bool CheckMyTeam(AActor* InOtherActor);

protected:
	//이 충돌체가 어떻게 동작하는지에 대한 정보
	FSkillCollisionData CollisionData;

protected:
	class AActor* SkillCauser;  // 이 충돌체가 생성되는 원인격
	class ACharacter* OwnerCharacter;	// 스킬 시전자 
	
protected:
	int32 Index;
	TArray<FHitData> HitDatas;

protected:
	FTimerHandle CollisionTimerHandle;

protected:
	TArray<AActor*> Ignores;
	TArray<AActor*> Hitted;

	// IAttackInterface을(를) 통해 상속됨
	AActor* GetDamageSource() const override;
};
