#include "Buffs/CBuffStructures.h"
#include "Global.h"

void FStatBuff::SetBuffName()
{
	FString mid = "";
	if (Value > 0.0f)
		mid = TEXT("증가");
	else
		mid = TEXT("감소");

	FString name = TEXT("");
	switch (StatType)
	{
		case ECharStatType::Attack:
			name = TEXT("공격력");
			break; 
		case ECharStatType::AttackSpeed:
			name = TEXT("공격속도");
			break;
		case ECharStatType::Defense:
			name = TEXT("방어력");
			break;
		case ECharStatType::CriticalRate:
			name = TEXT("치명타확률");
			break;
		case ECharStatType::CriticalDamage:
			name = TEXT("치명타데미지");
			break;
		case ECharStatType::Speed:
			name = TEXT("이동속도");
			break;
	}
	name = name + " " + mid;
	//BuffName = FName(*name);
}

void FStatBuff::SetBuffDesc()
{
	FString first = TEXT("");
	switch (StatType)
	{
		case ECharStatType::Attack:
			first = TEXT("공격력");
			break;
		case ECharStatType::AttackSpeed:
			first = TEXT("공격속도");
			break;
		case ECharStatType::Defense:
			first = TEXT("방어력");
			break;
		case ECharStatType::CriticalRate:
			first = TEXT("치명타확률");
			break;
		case ECharStatType::CriticalDamage:
			first = TEXT("치명타데미지");
			break;
		case ECharStatType::Speed:
			first = TEXT("이동속도");
			break;
	}

	FString value = ""; 
	value = FString::SanitizeFloat(Value);

	FString last = ""; 
	if (Value > 0.0f)
		last = TEXT("증가");
	else
		last = TEXT("감소");

	//BuffDesc = first + " " + value + last;
}
