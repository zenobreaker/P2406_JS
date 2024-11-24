#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CWeaponComponent.h"
#include "Skill/CSkillStructures.h"
#include "CSkillData.generated.h"

/// <summary>
/// ������ ĳ���Ͱ� �����Ͽ� ����� ��ų ����
/// </summary>
UCLASS()
class P2406_JS_API UCSkillData : public UObject
{
	GENERATED_BODY()

private:
	friend class UCSkillAsset;

private: 
	UPROPERTY(EditAnywhere, Category = "Skill Info")
	FSkillInfo SkillInfo;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSkillActionData> DoActionDatas;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FHitData> HitDatas;


public:
	void UseSkill();

};
