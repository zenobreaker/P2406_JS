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

public:
	void Pressed() override;

public:
	void Begin_DoSubAction_Implementation() override;
	void End_DoSubAction_Implementation() override;

public:
	UFUNCTION()
	virtual void OnAttachmentEndCollision();

	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);


private:
	int32 Index; 
	TArray<class ACharacter*> Hitted;

};
