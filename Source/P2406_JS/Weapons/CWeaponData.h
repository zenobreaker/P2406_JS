#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"


UCLASS()
class P2406_JS_API UCWeaponData : public UObject
{
	GENERATED_BODY()
	

private:
	friend class UCWeaponAsset;
	friend class UCActionDataAsset;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
	FORCEINLINE class UCSubAction* GetSubAction() { return SubAction; }
	FORCEINLINE const TArray<class UCActiveSkill*> GetSkills() { return Skills; }
	FORCEINLINE class UCDoAction* GetJumpAction() { return JumpAction; }
private:
	UPROPERTY()
	class ACAttachment* Attachment;

	UPROPERTY()
	class UCEquipment* Equipment;

	UPROPERTY()
	class UCDoAction* DoAction;

	UPROPERTY()
	class UCSubAction* SubAction;

	UPROPERTY()
	TArray<class UCActiveSkill*> Skills;

	UPROPERTY()
	class UCDoAction* JumpAction; 
};
