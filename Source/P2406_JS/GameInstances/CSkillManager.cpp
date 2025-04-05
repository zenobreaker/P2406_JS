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
	// �̹� �ش� ��ų�� ��ٿ� ���̺� �����ϴ��� üũ
	if (CooldownTable.Contains(InSkillID))
	{
		// ��ٿ��� ���� ��� ���� ����
		CooldownTable[InSkillID] = InCooldown;
		FLog::Log("this refresh " + FString::FromInt(InSkillID));
	}
	else
	{
		// �������� ������ ���� �߰�
		CooldownTable.Add(InSkillID, InCooldown);
	}
}

bool UCSkillManager::IsCompleteCooldown(int32 InSkillID)
{
	if (CooldownTable.Contains(InSkillID))
		return CooldownTable[InSkillID] <= 0.0f;
	else if(InSkillID > 0 )	// �͹��� ���� ���� ���� �ȵ�
		return true; 

	return false; 
}
