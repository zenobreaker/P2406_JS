#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buffs/CBuffStructures.h"
#include "CBuffManager.generated.h"


UCLASS()
class P2406_JS_API UCBuffManager : public UObject
{
	GENERATED_BODY()


public:
	UPROPERTY(EditDefaultsOnly, Category = "Buffs")
	TArray<struct FStatBuff> BuffDatabase; 

public:
	void BeginPlay(); 

	void CreatRandomBuffList(int32 InCount, TArray<struct FStatBuff>& InBuffs);

	void ApplyBuffsToPlayer(class ACPlayer* InPlayer, const struct FStatBuff& InBuff);
	void RemoveBuffsToPlayer(class ACPlayer* InPlayer,  const struct FStatBuff& InBuff);

private:
	TArray<struct FStatBuff> PlayerBuffs;
	TMap<class ACEnemy_AI*, TArray<struct FStatBuff>> EnemyBuffs; // 적 개별 버프



};
