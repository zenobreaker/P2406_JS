#include "GameInstances/CSkillManager.h"
#include "Global.h"

void UCSkillManager::UpdateAllCooldown(float InDeltaTime)
{
	for (auto& skill : CooldownTable)
	{
		UpdateCooldown(skill.Key, InDeltaTime);
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
			CLog::Print(FString::SanitizeFloat(InSkillID) + " : " + FString::SanitizeFloat(CooldownTable[InSkillID]), 1, 10.0f, FColor::Red);
		}

		if(OnUpdateCooldown.IsBound())
			OnUpdateCooldown.Broadcast(InSkillID, CooldownTable[InSkillID]);
	}
		
}

void UCSkillManager::ExecuteSkill(int32 InSkillID, float InCooldown)
{
	// �̹� �ش� ��ų�� ��ٿ� ���̺��� �����ϴ��� üũ
	if (CooldownTable.Contains(InSkillID))
	{
		// ��ٿ��� ���� ��� ���� ����
		CooldownTable[InSkillID] = InCooldown;
		CLog::Log("this refresh " + FString::FromInt(InSkillID));
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