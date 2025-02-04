#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AddOns/AttackInterface.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttachmentEndCollision);


DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FAttachmentBeginOverlap, class  ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);

UCLASS()
class P2406_JS_API ACAttachment
	: public AActor
	, public IAttackInterface
{
	GENERATED_BODY()
	

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Soket")
	FName TraceGoalName = "";

	// 이 Attachment 자체적으로 공격할 수 있는지??
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Attackable")
	bool bAttackable = false; 

public:
	FORCEINLINE FName GetTraceGoalName() { return TraceGoalName; }

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

	void HandleAttachmentOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

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
/// <summary>
/// IAttackInterface Func 
/// </summary>
	virtual bool CanAttack() const override;
	virtual bool IsEnable() const override;
	virtual AActor* GetDamageSource() const override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game")
	class ACharacter* OwnerCharacter;

protected:
	TArray<class UShapeComponent*> Collisions;

	bool bEnable = false; 
};
