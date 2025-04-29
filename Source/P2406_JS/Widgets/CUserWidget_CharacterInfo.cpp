#include "Widgets/CUserWidget_CharacterInfo.h"
#include "Global.h"
#include "Gameframework/Character.h"

void UCUserWidget_CharacterInfo::SetStat(ACharacter* InOwer)
{
	CheckNull(InOwer); 

	Stat = FHelpers::GetComponent<UCStatComponent>(InOwer);
	CheckNull(Stat); 

	StatusInfo = Stat->GetStatusData(); 

	// 이 함수는 블루프린트에서 부모 위젯이 열릴 때 호출 되므로 이벤트 할당 해제 
	REGISTER_EVENT_WITH_REPLACE(Stat, OnUpdatedStatusData, this, UCUserWidget_CharacterInfo::DrawInfo);
}


