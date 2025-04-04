#include "Components/CStatComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CBuffComponent.h"


UCStatComponent::UCStatComponent()
{
	//PrimaryComponentTick.bCanEverTick = true;

}


void UCStatComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner()); 
}

void UCStatComponent::ApplyBuff(const FStatBuff& InStatBuff)
{
	ApplyBuff(InStatBuff.StatType, InStatBuff.Value);
}

void UCStatComponent::ApplyBuff(ECharStatType  StatType, float InValue)
{
	FString debugName =  StaticEnum<ECharStatType>()->GetNameStringByValue((int64)StatType);
	FLog::Log("Stat : Apply Buff "+ debugName);
	switch (StatType)
	{
		case ECharStatType::Attack:
			BuffedAttack += InValue;
			break;
		case ECharStatType::AttackSpeed:
			BuffedAttackSpeed += InValue;
			break;
		case ECharStatType::Defense:
			BuffedDefense += InValue;
			break;
		case ECharStatType::Speed:
			BuffedSpeed += InValue;
			break;
		case ECharStatType::CriticalRate:
			BuffedCriticalRate += InValue;
			break;
		case ECharStatType::CriticalDamage:
			BuffedCriticalDamage += InValue;
			break;
	}
}

void UCStatComponent::RemoveBuff(const FStatBuff& InStatBuff)
{
	RemoveBuff(InStatBuff.StatType, InStatBuff.Value);
}

void UCStatComponent::RemoveBuff(ECharStatType  StatType, float InValue)
{
	switch (StatType)
	{
		case ECharStatType::Attack:
			BuffedAttack -= InValue;
			break;
		case ECharStatType::AttackSpeed:
			BuffedAttackSpeed -= InValue;
			break;
		case ECharStatType::Defense:
			BuffedDefense -= InValue;
			break;
		case ECharStatType::Speed:
			BuffedSpeed -= InValue;
			break;
		case ECharStatType::CriticalRate:
			BuffedCriticalRate -= InValue;
			break;
		case ECharStatType::CriticalDamage:
			BuffedCriticalDamage -= InValue;
			break;
	}
}

float UCStatComponent::GetStatValue(ECharStatType  StatType) const
{

	switch (StatType)
	{
		case ECharStatType::Attack:        return BaseAttack + BuffedAttack;
		case ECharStatType::Defense:       return BaseDefense + BuffedDefense;
		case ECharStatType::AttackSpeed:   return BaseAttackSpeed + BuffedAttackSpeed;
		case ECharStatType::CriticalRate:  return BaseCriticalRate + BuffedCriticalRate;
		case ECharStatType::CriticalDamage:return BaseCriticalDamage + BuffedCriticalDamage;
		case ECharStatType::Speed:         return BuffedSpeed; // 기본 속도는 MovementComponent에서 가져올 수도 있음
		default: return 0.f;
	}

	return 0.f;
}
