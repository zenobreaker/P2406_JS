#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CWeaponStructures.h"
#include "AddOns/AttackInterface.h"
#include "AddOns/IWeaponTrace.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class  ACharacter*, InAttacker, AActor*, InAttackCauser, class AActor*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class AActor*, InOther);

UCLASS()
class P2406_JS_API ACAttachment
	: public AActor
	, public IAttackInterface
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly, Category = "Trail")
	FAttachmentTrailData TrailData;

	UPROPERTY(EditDefaultsOnly, Category = "Soket")
	FAttachmentSocketData SocketData;

	// 이 Attachment 자체적으로 공격할 수 있는지??
	UPROPERTY(EditAnywhere, Category = "Attackable")
	bool bAttackable = false;

public:
	FORCEINLINE const FAttachmentTrailData& GetTrailData() { return TrailData; }
	FORCEINLINE const FAttachmentSocketData& GetSocketData() { return SocketData; }
	FORCEINLINE FName GetSocketStartName() const { return SocketData.StartSocket; }
	FORCEINLINE FName GetSocketGoalName() const { return SocketData.EndSocket; }

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* Root;

public:
	ACAttachment();

protected:
	virtual void BeginPlay() override;

public:
	void OnCollisions();
	void OffCollisions();

	//A
	void HandleAttachmentOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class AActor* InOther);

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnUnequip();
	virtual void OnUnequip_Implementation() {}

protected:
	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachTo(FName InSocketName);

	UFUNCTION(BlueprintCallable, Category = "Attach")
	void AttachToCollision(FName InCollisionName);

private:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	UFUNCTION()
	void OnDestroy();

public:
	FAttachmentBeginCollision OnAttachmentBeginCollision;
	FAttachmentEndCollision OnAttachmentEndCollision;

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;

public:
	virtual bool CanAttack() const override;
	virtual bool IsEnable() const override;
	virtual AActor* GetDamageSource() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	class ACharacter* OwnerCharacter;

protected:
	TArray<class UShapeComponent*> Collisions;
	class UPrimitiveComponent* Mesh;

	bool bEnable = false;
};
