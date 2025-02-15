#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Skill/CSkillStructures.h"
#include "Weapons/CWeaponStructures.h"
#include "Weapons/AddOns/AttackInterface.h"
#include "CSkillCollision.generated.h"

UCLASS(Abstract)
class P2406_JS_API ACSkillCollision
	: public AActor
	, public IAttackInterface
{
	GENERATED_BODY()

public:
	ACSkillCollision();

public:
	FORCEINLINE void AddIgnore(AActor* InActor) { Ignores.Add(InActor); }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPrimitiveComponent* CollisionComponent;

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void SetSkillOwnerData(class ACharacter* InOwner, const TArray<FHitData>& InHitDatas);

	// 충돌 시작
	UFUNCTION()
	virtual void ActivateCollision() {}

	// 충돌 종료
	UFUNCTION()
	virtual void DeactivateCollision() {}

	UFUNCTION()
	virtual void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//virtual void EndSkillCollisionProcess();

protected:
	// 충돌이 발생했을 때 처리할 이벤트
	UFUNCTION()
	virtual void HandleCollision(AActor* HitActor);

	UFUNCTION()
	virtual void DestroyProcess();

private:
	bool CheckMyTeam(AActor* InOtherActor);

public:
	TArray<AActor*> Hitted;

protected:
	class ACharacter* OwnerCharacter;

	int32 Index;
	TArray<FHitData> HitDatas;
	TArray<AActor*> Ignores;

protected:
	FTimerHandle CollisionTimerHandle;
	// IAttackInterface을(를) 통해 상속됨
	virtual AActor* GetDamageSource() const override;

};
