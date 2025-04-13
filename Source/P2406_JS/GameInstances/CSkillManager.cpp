#include "GameInstances/CSkillManager.h"
#include "Global.h"

void UCSkillManager::UpdateAllCooldown(float InDeltaTime)
{
	CheckFalse(CooldownTable.Num() > 0);

	TArray<int32> SkillIDs;
	
	{
		FScopeLock Lock(&Mutex);
		CooldownTable.GetKeys(SkillIDs);
	}

	if (SkillIDs.Num() == 0)
		return; 

	for (const int32& skillID : SkillIDs)
	{
		UpdateCooldown(skillID, InDeltaTime);
	}
}

void UCSkillManager::UpdateCooldown(int32 InSkillID, float InDeltaTime)
{
	FScopeLock Lock(&Mutex);
	if (CooldownTable.Contains(InSkillID) == true)
	{
		CooldownTable[InSkillID] -= InDeltaTime;

		CooldownTable[InSkillID] = FMath::Max(CooldownTable[InSkillID], 0.0f); 

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
