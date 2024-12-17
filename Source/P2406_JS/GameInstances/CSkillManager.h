
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSkillManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateCooldown, int32, InSkillID, float, InDeltaTime);

UCLASS()
class P2406_JS_API UCSkillManager : public UObject
{
	GENERATED_BODY()

public:
	bool IsCompleteCooldown(int32 InSkillID);

public:

	void UpdateAllCooldown(float InDeltaTime);
	void UpdateCooldown(int32 InSkillID, float InDeltaTime);

public:
	void ExecuteSkill(int32 InSkillID, float InCooldown);
public:
	// 스킬 델리게이트 
	FOnUpdateCooldown OnUpdateCooldown;

private:
	TMap<int32, float> CooldownTable;
};
