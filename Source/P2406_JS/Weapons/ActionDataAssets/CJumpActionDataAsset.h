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
	TArray<FDoActionData> DownAttackActionDatas;

	UPROPERTY(EditAnywhere, Category = "DownAction")
	TArray<FHitData> DownAttackHitDatas;

public:
	UCJumpActionDataAsset(); 

	void BeginPlay(class ACharacter* InOwner);
};
