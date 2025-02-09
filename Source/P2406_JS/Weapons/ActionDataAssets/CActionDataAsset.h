#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "CActionDataAsset.generated.h"


UCLASS()
class P2406_JS_API UCActionDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
private:
	// 직렬화 우선순위 보장요 protected로 했는데 직렬화가 안되서 안뜨니까 수정할 대 참고
	//UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas;

public:
	UCActionDataAsset();

	virtual void BeginPlay(class ACharacter* InOwner,class UCWeaponData* InParentData, class UCDoAction** OutJumpDoAction);
};
