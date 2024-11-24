#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CSkillAsset.generated.h"

UCLASS()
class P2406_JS_API UCSkillAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Skill")
	FString SkillName; 

	UPROPERTY(EditAnywhere, Category = "Skill")
	float Cost;

	UPROPERTY(EditAnywhere, Category = "Skill")
	float Cooldown; 

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FDoActionData> ActionDatas;

	UPROPERTY(EditAnywhere, Category = "Skill")
	TArray<FHitData> HitDatas;
};
