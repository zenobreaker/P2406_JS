#include "Widgets/CUserWidget_CharacterInfo.h"
#include "Global.h"
#include "Gameframework/Character.h"

void UCUserWidget_CharacterInfo::SetStat(ACharacter* InOwer)
{
	CheckNull(InOwer); 

	Stat = FHelpers::GetComponent<UCStatComponent>(InOwer);
	CheckNull(Stat); 

	StatusInfo = Stat->GetStatusData(); 


	StatusUIInfo.AttackText				= GetValueToText(StatusInfo.Attack, false);
	StatusUIInfo.AttackSpeedText		= GetValueToText(StatusInfo.AttackSpeed, true);
	StatusUIInfo.DefenseText			= GetValueToText(StatusInfo.Defense, false);
	StatusUIInfo.CritText				= GetValueToText(StatusInfo.CriticalRate, true);
	StatusUIInfo.CritDmgText			= GetValueToText(StatusInfo.CriticalDamage, true);
	StatusUIInfo.SpeedText				= GetValueToText(StatusInfo.Speed, false);

	StatusUIInfo.BuffedAttackText		= GetValueToText(StatusInfo.BuffedAttack, false);
	StatusUIInfo.BuffedAttackSpeedText	= GetValueToText(StatusInfo.BuffedAttackSpeed, true);
	StatusUIInfo.BuffedDefenseText		= GetValueToText(StatusInfo.BuffedDefense, false);
	StatusUIInfo.BuffedCritText			= GetValueToText(StatusInfo.BuffedCriticalRate, true);
	StatusUIInfo.BuffedCritDmgText		= GetValueToText(StatusInfo.BuffedCriticalDamage, true);
	StatusUIInfo.BuffedSpeedText		= GetValueToText(StatusInfo.BuffedSpeed, false);


	// 이 함수는 블루프린트에서 부모 위젯이 열릴 때 호출 되므로 이벤트 할당 해제 cbn
	REGISTER_EVENT_WITH_REPLACE(Stat, OnUpdatedStatusData, this, UCUserWidget_CharacterInfo::DrawInfo);
}

FText UCUserWidget_CharacterInfo::GetValueToText(float InValue, bool bPercent)
{
	FNumberFormattingOptions formatOptions;

	formatOptions.MinimumIntegralDigits = 1;
	formatOptions.MaximumFractionalDigits = 0;
	formatOptions.RoundingMode = ERoundingMode::HalfFromZero;

	float value = bPercent ? InValue * 100.0f : InValue;
	if (value == 0.0f)
		return FText::GetEmpty();

	FText text = FText::AsNumber(value, &formatOptions);

	return text; 
}


