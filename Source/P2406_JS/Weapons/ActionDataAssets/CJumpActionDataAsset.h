#pragma once

#include "CoreMinimal.h"
#include "Weapons/ActionDataAssets/CActionDataAsset.h"
#include "CJumpActionDataAsset.generated.h"


UCLASS()
class P2406_JS_API UCJumpActionDataAsset : public UCActionDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category ="DownAction")
	TArray<FDoActionData> FallAttackActionDatas;

	UPROPERTY(EditAnywhere, Category = "DownAction")
	TArray<FHitData> FallAttackHitDatas;

public:
	UCJumpActionDataAsset(); 

	virtual void BeginPlay(class ACharacter* InOwner,
		class UCWeaponData* InParentData,
		class UCDoAction** OutJumpDoAction) override;
};
