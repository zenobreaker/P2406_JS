#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "CSubAction_Sword.generated.h"


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
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);


private:
	// �� ��ġ ����
	void TrackEnemyHeight();
	// ĳ���� ���� �̵�
	void TeleportToEnemy(class ACharacter* InTargetEnemy);
	// ���� ������ ���߱�
	void StopEnemyMovement(class ACharacter* InTargetEnemy);
	// ���� ���� ��ȯ
	void ChangeEnemyState(class ACharacter* InTargetEnemy);

	void StopMovement();
	void ChangeState();


private:
	int32 Index; 
	TArray<class ACharacter*> Hitted;
	FTimerHandle AirSlashTimeHandle;
	FTimerHandle TrackEnemyTimeHandle;
	FTimerHandle EnemyStateChangeHandle;
};
