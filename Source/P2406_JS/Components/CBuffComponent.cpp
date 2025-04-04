#include "Components/CBuffComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStatComponent.h"

UCBuffComponent::UCBuffComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UCBuffComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCBuffComponent::ApplyBuff(const FStatBuff& NewBuff)
{
	CheckNull(OwnerCharacter); 
	CheckTrue(NewBuff.StatType == ECharStatType::Max);
	UCStatComponent* stat = FHelpers::GetComponent<UCStatComponent>(OwnerCharacter);
	CheckNull(stat);

	for (FStatBuff& buff : ActiveBuffs)
	{
		if (buff.StatType == NewBuff.StatType)
		{
			buff.Value += NewBuff.Value;
			stat->ApplyBuff(buff);
			return; 
		}
	}

	ActiveBuffs.Add(NewBuff);
	stat->ApplyBuff(NewBuff);

	if (NewBuff.Duration > 0.f)
	{
		FTimerHandle TimerHandle; 
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			FTimerDelegate::CreateUObject(this, &UCBuffComponent::OnBuffExpired,
				NewBuff.StatType, NewBuff.Value), NewBuff.Duration, false);
		
	}
}

void UCBuffComponent::RemoveBuff(const FStatBuff& InStatBuff)
{
	RemoveBuff(InStatBuff.StatType, InStatBuff.Value);
}

void UCBuffComponent::RemoveBuff(const ECharStatType StatType, float Value)
{
	CheckNull(OwnerCharacter);
	CheckTrue(StatType == ECharStatType::Max);
	UCStatComponent* stat = FHelpers::GetComponent<UCStatComponent>(OwnerCharacter);
	CheckNull(stat);

	for (int32 i = 0; i < ActiveBuffs.Num(); i++)
	{
		if (ActiveBuffs[i].StatType == StatType)
		{
			ActiveBuffs[i].Value -= Value;
			stat->RemoveBuff(StatType, Value);
			
			if (ActiveBuffs[i].Value <= 0.0f)
			{
				ActiveBuffs.RemoveAt(i);
			}

			return; 
		}
	}
}

void UCBuffComponent::RemoveExpiredBuffs()
{
}

void UCBuffComponent::ClearAllBuffs()
{
}

void UCBuffComponent::OnBuffExpired(ECharStatType InStatType, float Value)
{
	RemoveBuff(InStatType, Value);
}


