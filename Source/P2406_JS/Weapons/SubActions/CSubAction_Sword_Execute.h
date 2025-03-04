#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "CSubAction_Sword_Execute.generated.h"

UCLASS(Blueprintable)
class P2406_JS_API UCSubAction_Sword_Execute : public UCSubAction
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnyWhere, Category = "Action")
	float Distance = 200.0f; 

	UPROPERTY(EditAnyWhere, Category = "Action")
	TArray<FDoActionData> ActionDatas;

	UPROPERTY(EditAnyWhere, Category = "Action")
	TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere, Category = "Action")
	class UParticleSystem* ExecuteEffect;

	UPROPERTY(EditAnywhere, Category = "Action")
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Action")
	FVector EffectScale = FVector::OneVector;



public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;

public:
	void Pressed() override;
	

private:
	class ACharacter* CheckFrontNearTarget();

	void DoExecute();

private:
	class ACharacter* Target; 

	bool bExecuting = false;
};
