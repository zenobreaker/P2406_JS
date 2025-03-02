#include "Components/CPatternComponent.h"
#include "Global.h"
#include "Engine/DataTable.h"

#include "Gameframework/Character.h"
#include "GameInstances/CGameInstance.h"
#include "GameInstances/CPatternConditionManager.h"
#include "Skill/CSkillAsset.h"

UCPatternComponent::UCPatternComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	//FHelpers::GetAsset<UDataTable>(&PatternDataTable, "");

	//int maxPhase = 0; 
	//if (!!PatternDataTable)
	//{
	//	TArray<FPatternData*> Rows;
	//	PatternDataTable->GetAllRows("Pattern Data String", Rows);
	//	for (const FPatternData* row : Rows)
	//	{
	//		if (row != nullptr)
	//		{
	//			maxPhase = FMath::Max(maxPhase, row->Phase);
	//			Patterns.AddUnique(*row);
	//		}
	//	}
	//}

	//MaxPhase = maxPhase;
}


void UCPatternComponent::BeginPlay()
{
	Super::BeginPlay();

	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (instance != nullptr)
	{
		PatternCondition = instance->PatternCondition;
	}


	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	for (const FPatternData& data : PatternDatas)
	{
		FPatternInfo info; 
		info.PatternID = data.PatternID;
		info.Phase = data.Phase;
		info.Priority = data.Priority;
		info.ConditionIDs = data.ConditionIDs;
		if (data.SkillAssets.Num() > 0)
		{
			for (int32 i = 0; i < data.SkillAssets.Num(); i++)
			{
				if (data.SkillAssets[i] == nullptr)
					continue; 

				UCActiveSkill* skill = nullptr;
				data.SkillAssets[i]->SkillAsset_BeginPlay(OwnerCharacter, &skill);

				info.ActiveSkills.Add(skill);
			}
		}

		PatternInfos.Add(info);
	}
}


void UCPatternComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 패턴 별 쿨타임 돌리기
	CheckFalse(PatternInfos.Num() > 0);
	
	for (int32 i = 0; i < PatternInfos.Num(); i++)
	{
		for (auto& skill : PatternInfos[i].ActiveSkills)
		{
			if(skill->IsCooldown() == false)
				skill->Update_Cooldown(DeltaTime); 
		}
	}
}

void UCPatternComponent::SetPattern(int32 InPaternID)
{
	//TODO : 
}

void UCPatternComponent::ExecutePattern()
{	
	CheckNull(DecidedPattern);
	CheckFalse(DecidedPattern->ActiveSkills.Num() <= 0);
	
	FLog::Print("Execute Pattern : " + FString::FromInt(DecidedPattern->PatternID));

	//TODO: 각 스킬들을 한꺼번에 실행하는 구조인데 어디다가 딜레이값을 넣어야할지도 모르겠다. 
	for (int32 i = 0; i < DecidedPattern->ActiveSkills.Num(); i++)
	{
		DecidedPattern->ActiveSkills[i]->ExecuteSkill();
	}

	DecidedPattern = nullptr;
	bDecided = false;
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnDecidedPattern, bDecided);
}

void UCPatternComponent::DecidePattern()
{
	CheckTrue(bDecided);

	TArray<FPatternInfo*> selectedInfos;

	for (FPatternInfo& Pattern : PatternInfos)
	{
		bool bCanExecute = true;

		for (int32 ConditionID : Pattern.ConditionIDs)
		{
			if (!!PatternCondition && PatternCondition->CheckCondition(ConditionID, OwnerCharacter) == false)
			{
				bCanExecute = false;
				break;
			}
		}

		if (bCanExecute)
		{
			selectedInfos.HeapPush(&Pattern, [=](FPatternInfo& a, FPatternInfo& b)
				{
					return a.Priority > b.Priority;
				});
		}
	}

	if (selectedInfos.Num() <= 0)
		return;

	bDecided = true;
	DecidedPattern = selectedInfos.HeapTop();
	DYNAMIC_EVENT_CALL_ONE_PARAM(OnDecidedPattern, bDecided);
	FLog::Print("Decide Pattern : " + FString::FromInt(DecidedPattern->PatternID));
}

