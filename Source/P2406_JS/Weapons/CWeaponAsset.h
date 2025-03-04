#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Weapons/CWeaponStructures.h"
#include "Skill/CSkillAsset.h"
#include "ActionDataAssets/CJumpActionDataAsset.h"
#include "CWeaponAsset.generated.h"


UCLASS()
class P2406_JS_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCSubAction> SubActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere)
	TArray<UCSkillAsset*> ActiveSkillAssets;

	UPROPERTY(EditAnywhere)
	UCActionDataAsset* JumpAcitonAsset; 

	// 비상!! SubAction ㅂㅐ열로 하면 기존에 로직들 죄다 수정해야함 비상!!
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCSubAction> ExecuteActionClass;


public:
	UCWeaponAsset();

	void WA_BeginPlay(class ACharacter* InOwner, class UCWeaponData** OutWeaponData);
};
