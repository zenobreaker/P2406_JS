#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Skill/CSkillAsset.h"
#include "CPatternStructures.generated.h"

USTRUCT(BlueprintType)
struct FPatternData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int32 PatternID = 0;
	
	UPROPERTY(EditAnywhere)
	int32 Phase = 0;

	UPROPERTY(EditAnywhere)
	float Priority = 0.0f;

	UPROPERTY(EditAnywhere)
	TArray<int32> ConditionIDs;

	UPROPERTY(EditAnywhere)
	TArray<UCSkillAsset*> SkillAssets;


};

class P2406_JS_API UCPatternStructures : public UObject
{
};
