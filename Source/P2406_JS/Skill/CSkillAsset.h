#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Skill/CSkillStructures.h"
#include "CSkillAsset.generated.h"

UCLASS()
class P2406_JS_API UCSkillAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	FSkillInfo SkillInfo;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FSkillActionData> DoActionDatas;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FHitData> HitDatas;

public:
	UCSkillAsset(); 

	void BeginPlay(class ACharacter* InOwner, class UCActiveSkill** OutActiveSkill);
};
