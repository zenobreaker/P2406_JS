#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Skill/CSkillStructures.h"
#include "Weapons/AddOns/AttackInterface.h"
#include "CSkillCollisionComponent.generated.h"

DECLARE_DELEGATE(FOnSkillDamaged);
DECLARE_DELEGATE_OneParam(FOnSkillDamagedOneParam, ACharacter*);
DECLARE_DELEGATE_ThreeParams(FOnSkillDamagedThreeParams, ACharacter*, AActor*, ACharacter*);

UCLASS(Abstract)
class P2406_JS_API UCSkillCollisionComponent
	: public USceneComponent
	, public IAttackInterface
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere)
	bool bDrawDebug = false;

public:
	FORCEINLINE FSkillCollisionData& GetCollisionData() {return CollisionData;}
	FORCEINLINE void AddIgnore(AActor* InActor) { Ignores.Add(InActor); }
	FORCEINLINE int32 GetDamagedCount() const { return Hitted.Num(); }
	FORCEINLINE void SetDrawDebug(bool InValue) { bDrawDebug = InValue; }
	FORCEINLINE bool GetDrawDebug() const { return bDrawDebug; }

protected:
	virtual void BeginPlay() override;

public:
	void SetCollisionData(class ACharacter* InOwner, FSkillCollisionData& InCollisinData);
	void SetCollisionData(class ACharacter* InOwner, FSkillCollisionData& InCollisinData, class ACSkillEntity* InEntity);

public:
	// 충돌 시작
	virtual void ActivateCollision(int32 InInex = 0) {} /*PURE_VIRTUAL(UCSkillCollisionComponent::ActivateCollision, );*/
	// 충돌 종료
	virtual void DeactivateCollision(int32 InInex = 0) {}/*PURE_VIRTUAL(UCSkillCollisionComponent::DeactivateCollision, );*/
	// 충돌 검사 
	virtual void CheckCollision() {} /*PURE_VIRTUAL(UCSkillCollisionComponent::CheckCollision, );*/


protected:
	virtual void DrawDebugCollisionLine() {}

public:
	TArray<FOnSkillDamaged> OnSkillDamageds;
	TArray<FOnSkillDamagedOneParam> OnSkillDamagedOneParams;
	TArray<FOnSkillDamagedThreeParams> OnSkillDamagedThreeParams;

protected:
	bool CheckMyTeam(AActor* InOtherActor);

protected:
	//이 충돌체가 어떻게 동작하는지에 대한 정보
	UPROPERTY()
	FSkillCollisionData CollisionData;


	// IAttackInterface을(를) 통해 상속됨
	AActor* GetDamageSource() const override;

protected:
	class AActor* SkillCauser;  // 이 충돌체가 생성되는 원인격
	class ACharacter* OwnerCharacter;	// 스킬 시전자 
	class ACSkillEntity* Entity;

protected:
	int32 Index;
	TArray<FHitData> HitDatas;
	bool bActivate = false; 


protected:
	FTimerHandle CollisionTimerHandle;

protected:
	TArray<AActor*> Ignores;
	TArray<AActor*> Hitted;

};
