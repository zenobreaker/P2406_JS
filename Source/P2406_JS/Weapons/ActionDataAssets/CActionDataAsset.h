#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CActionDataAsset.generated.h"


UCLASS()
class P2406_JS_API UCActionDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas;
};
