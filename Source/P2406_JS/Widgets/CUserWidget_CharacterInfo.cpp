#include "Widgets/CUserWidget_CharacterInfo.h"
#include "Global.h"
#include "Gameframework/Character.h"

void UCUserWidget_CharacterInfo::SetStat(ACharacter* InOwer)
{
	CheckNull(InOwer); 

	Stat = FHelpers::GetComponent<UCStatComponent>(InOwer);
	CheckNull(Stat); 

	StatusInfo = Stat->GetStatusData(); 

	// �� �Լ��� �������Ʈ���� �θ� ������ ���� �� ȣ�� �ǹǷ� �̺�Ʈ �Ҵ� ���� 
	REGISTER_EVENT_WITH_REPLACE(Stat, OnUpdatedStatusData, this, UCUserWidget_CharacterInfo::DrawInfo);
}


