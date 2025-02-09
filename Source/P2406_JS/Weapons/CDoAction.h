#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponStructures.h"
#include "CDoAction.generated.h"

UCLASS()
class P2406_JS_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	FORCEINLINE bool GetInAction() { return bInAction; }
	FORCEINLINE bool GetInExtraAction() const { return bInExtraAction; }
	
public:
	UCDoAction();

	virtual void BeginPlay
	(
		class ACharacter* InOwner,
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		const TArray<FDoActionData>& InDoActionDatas,
		const TArray<FHitData>& InHitDatas
	);

	virtual void Tick(float InDeltaTime) {}

public:
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

public:
	virtual void SetHeavyActionFlag(bool InValue) {};
	
protected:
	ACharacter* FindBestTarget(); 

	bool IsMyTeam(class ACharacter* InAttacker, class ACharacter* InOther);

	bool IsOtherIsMe(class ACharacter* InOther);

public: 
	virtual void PlayFallAttackMontage() {};

public:
	UFUNCTION()
	virtual void OnBeginEquip() {}

	UFUNCTION()
	virtual void OnUnequip() {}

public:
	UFUNCTION()
	virtual void OnAttachmentBeginCollision() {}

	UFUNCTION()
	virtual void OnAttachmentEndCollision() {}


	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther) {}

	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) {}

protected:
	bool bBeginAction;
	bool bInAction; 
	bool bInExtraAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;

protected:
	TArray<class ACharacter*> Hitted;
};

