#include "GameInstances/CPatternConditionManager.h"
#include "Global.h"
#include "GameFramework/Character.h"

#include "Components/CHealthPointComponent.h"

bool FPatternCondition::IsConditionMet(ACharacter* InCharacter)
{
	// 여기서 타입과 value로 비교 

	bool bResult = false;
	switch (ConditionType)
	{
	case EPatternConditionType::HealthBelow:
	{
		UCHealthPointComponent* health = Cast<UCHealthPointComponent>(InCharacter);
		if (health != nullptr)
		{
			if (health->GetHealth() < Value)
				bResult = true;
		}
	}
	break;
	case EPatternConditionType::TimeElapsed:
		break;
	case EPatternConditionType::BossDamaged:
		break;
	case EPatternConditionType::PlayerInRange:
		break;
	}

	return bResult;
}

//----------------------------------------------------------------------

UCPatternConditionManager::UCPatternConditionManager()
{
	FHelpers::GetAsset<UDataTable>(&DataTable, "/Script/Engine.DataTable'/Game/PatternCondition/DT_PatternConditionInfo.DT_PatternConditionInfo'");

}

void UCPatternConditionManager::InitConditionData()
{
	if (!ensure(DataTable))  // DataTable이 유효한지 체크
	{
		UE_LOG(LogTemp, Error, TEXT("[UCPatternConditionManager] DataTable is NULL! InitConditionData failed."));
		return;
	}


	TArray<FPatternConditionInfo*> Rows;
	DataTable->GetAllRows("Pattern Condition String", Rows);

	for (const FPatternConditionInfo* row : Rows)
	{
		if (row != nullptr)
		{
			AddCondition(row->ID, row->Type, row->Value);
		}
	}

}

void UCPatternConditionManager::AddCondition(int32 InConditionID,
	FString TriggerType, float InValue)
{
	for (int8 i = 0; i < (int8)EPatternConditionType::Max; i++)
	{
		if (TriggerType.Compare(StaticEnum<EPatternConditionType>()->GetNameStringByValue(i)))
		{
			TUniquePtr<FPatternCondition> pc = MakeUnique<FPatternCondition>((EPatternConditionType)i, InValue);
			PatternConditionTable.Add(InConditionID, MoveTemp(pc));
		}
	}
}

void UCPatternConditionManager::AddCondition(int32 InConditionID,
	EPatternConditionType InConditionType, float InValue)
{
	for (int8 i = 0; i < (int8)EPatternConditionType::Max; i++)
	{
		TUniquePtr<FPatternCondition> pc = MakeUnique<FPatternCondition>((EPatternConditionType)i, InValue);
		PatternConditionTable.Add(InConditionID, MoveTemp(pc));
	}
}

bool UCPatternConditionManager::CheckCondition(int32 InConditionID, ACharacter* InCharacter)
{
	CheckFalseResult(PatternConditionTable.Num() > 0, false);

	if (PatternConditionTable.Contains(InConditionID))
	{
		return PatternConditionTable[InConditionID]->IsConditionMet(InCharacter);
	}

	return false;
}

