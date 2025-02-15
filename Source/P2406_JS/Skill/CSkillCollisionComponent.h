#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Skill/CSkillStructures.h"
#include "Weapons/AddOns/AttackInterface.h"
#include "CSkillCollisionComponent.generated.h"

UCLASS()
class P2406_JS_API UCSkillCollisionComponent 
	: public UActorComponent
	, public IAttackInterface
{
	GENERATED_BODY()

public:
	FORCEINLINE void AddIgnore(AActor* InActor) { Ignores.Add(InActor); }

protected:
	virtual void BeginPlay() override;

public:
	void SetCollisionData(class ACharacter* InOwner, FSkillCollisionData& InCollisinData);

public:
	// �浹 ����
	virtual void ActivateCollision() {}
	// �浹 ����
	virtual void DeactivateCollision() {}
	// �浹 �˻� 
	virtual void CheckCollision() {}


private:
	bool CheckMyTeam(AActor* InOtherActor);

protected:
	//�� �浹ü�� ��� �����ϴ����� ���� ����
	FSkillCollisionData CollisionData;

protected:
	class AActor* SkillCauser;  // �� �浹ü�� �����Ǵ� ���ΰ�
	class ACharacter* OwnerCharacter;	// ��ų ������ 
	
protected:
	int32 Index;
	TArray<FHitData> HitDatas;

protected:
	FTimerHandle CollisionTimerHandle;

protected:
	TArray<AActor*> Ignores;
	TArray<AActor*> Hitted;

	// IAttackInterface��(��) ���� ��ӵ�
	AActor* GetDamageSource() const override;
};
