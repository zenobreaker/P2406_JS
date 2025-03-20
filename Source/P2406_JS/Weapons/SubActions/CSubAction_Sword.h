#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "CSubAction_Sword.generated.h"

USTRUCT()
struct FSlashData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* AttackEffect;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	FVector Location = FVector::OneVector;

	UPROPERTY(EditAnywhere)
	float forward = 100.0f;

	UPROPERTY(EditAnywhere)
	float angle = 0.0f;

	UPROPERTY(EditAnywhere)
	FVector Scale = FVector::OneVector;

public:
	void CreateSlashEffect(class ACharacter *InOwner);
	
};


UCLASS(Blueprintable)
class P2406_JS_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()
	
public:
	UCSubAction_Sword();

public:

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FDoActionData> ActionDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
	float TrackHeightValue = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Action")
	float OffsetDistance = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Action")
	TArray<FSlashData> SlashDatas;

public:
	void Pressed() override;


private:
	void OnPressSpecialAction();

public:
	void Begin_DoSubAction_Implementation() override;
	void End_DoSubAction_Implementation() override;

private:
	void SetInputSubAction();
	void EndInputSubAction();

public:
	UFUNCTION()
	virtual void OnAttachmentEndCollision();

	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class AActor* InOther);


private:
	// 적 위치 감지
	void TrackEnemyHeight();
	// 캐릭터 순간 이동
	void TeleportToEnemy(class ACharacter* InTargetEnemy);
	// 적의 움직임 멈추기
	void StopEnemyMovement(class ACharacter* InTargetEnemy);
	// 적의 상태 변환
	void ChangeEnemyState(class ACharacter* InTargetEnemy);

	void StopMovement();
	void ChangeState();

public:
	void TraceAttackArea();
	void CreateAttackSlash();

private:
	int32 Index; 
	TArray<class AActor*> Hitted;
	FTimerHandle AirSlashTimeHandle;
	FTimerHandle TrackEnemyTimeHandle;
	FTimerHandle EnemyStateChangeHandle;

	bool bSubAction = false;
	int32 Test = 0;
};
