#include "GameInstances/CSkillManager.h"
#include "Global.h"

void UCSkillManager::UpdateAllCooldown(float InDeltaTime)
{
	CheckFalse(CooldownTable.Num() > 0);
	if (CooldownTable.IsEmpty())
		return; 

	TArray<int32> SkillIDs;
	CooldownTable.GetKeys(SkillIDs);
	for (const int32& skillID : SkillIDs)
	{
		UpdateCooldown(skillID, InDeltaTime);
	}
}

void UCSkillManager::UpdateCooldown(int32 InSkillID, float InDeltaTime)
{
	if (CooldownTable.Contains(InSkillID) == true)
	{
		CooldownTable[InSkillID] -= InDeltaTime;

		CooldownTable[InSkillID] = FMath::Max(CooldownTable[InSkillID], 0.0f); 

		if (CooldownTable[InSkillID] > 0.0f)
		{
			//FLog::Print(FString::SanitizeFloat(InSkillID) + " : " + FString::SanitizeFloat(CooldownTable[InSkillID]), 1, 10.0f, FColor::Red);
		}

		if(OnUpdateCooldown.IsBound())
			OnUpdateCooldown.Broadcast(InSkillID, CooldownTable[InSkillID]);
	}
		
}

void UCSkillManager::ExecuteSkill(int32 InSkillID, float InCooldown)
{
	// 이미 해당 스킬이 쿨다운 테이블에 존재하는지 체크
	if (CooldownTable.Contains(InSkillID))
	{
		// 쿨다운이 있을 경우 값을 갱신
		CooldownTable[InSkillID] = InCooldown;
		FLog::Log("this refresh " + FString::FromInt(InSkillID));
	}
	else
	{
		// 존재하지 않으면 새로 추가
		CooldownTable.Add(InSkillID, InCooldown);
	}
}

bool UCSkillManager::IsCompleteCooldown(int32 InSkillID)
{
	if (CooldownTable.Contains(InSkillID))
		return CooldownTable[InSkillID] <= 0.0f;
	else if(InSkillID > 0 )	// 터무니 없는 값이 오면 안됨
		return true; 

	return false; 
}
