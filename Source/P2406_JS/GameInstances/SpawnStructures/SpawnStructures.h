#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpawnStructures.generated.h"


USTRUCT(BlueprintType)
struct FMonsterSpawnInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	uint32 MonsterID;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACEnemy_AI> MonsterClass;

	UPROPERTY(EditAnywhere)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere)
	FRotator SpawnRotation; 
};

USTRUCT(BlueprintType)
struct FStageWaveInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 StageID; 

	UPROPERTY(EditAnywhere)
    int32 WaveID;

    UPROPERTY(EditAnywhere)
    TArray<FMonsterSpawnInfo> Monsters;
};

USTRUCT(BlueprintType)
struct FStageInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	int32 StageID;

	UPROPERTY(EditAnywhere)
	TArray<FStageWaveInfo> WaveData;
};



UCLASS()
class P2406_JS_API USpawnStructures : public UObject
{
	GENERATED_BODY()
	
};
