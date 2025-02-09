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
	// ����ȭ �켱���� ����� protected�� �ߴµ� ����ȭ�� �ȵǼ� �ȶߴϱ� ������ �� ����
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
