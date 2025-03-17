#include "Components/CPatternComponent.h"
#include "Global.h"
#include "Engine/DataTable.h"

#include "Characters/CBoss_AI.h"
#include "Components/CStateComponent.h"
#include "Components/AddOns/CPatternDecider.h"
#include "Gameframework/Character.h"
#include "GameInstances/CGameInstance.h"
#include "GameInstances/CPatternConditionManager.h"
#include "Skill/CSkillAsset.h"

UCPatternComponent::UCPatternComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	FHelpers::GetClass<UCPatternDecider>(&PatternDeciderClass, L"/Script/Engine.Blueprint'/Game/Enemies/Boss/BP_CPatternDcider.BP_CPatternDcider_C'");
}


void UCPatternComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentPhase = 1;

	UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (instance != nullptr)
		PatternCondition = instance->PatternCondition;

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	PatternDecider = NewObject<UCPatternDecider>(this, PatternDeciderClass);
	if (PatternDecider != nullptr)
		PatternDecider->BeginPlay();

	//TODO : 여기 데이터가 제대로 입력안되는 버그 있음
	for (const FPatternData& data : PatternDatas)
	{
		FPatternInfo info;
		info.PatternID = data.PatternID;
		info.Phase = data.Phase;
		info.Priority = data.Priority;
		info.ActionRange = data.ActionRange;
		info.ConditionIDs = data.ConditionIDs;

		if (data.SkillAsset != nullptr)
		{
			UCActiveSkill* skill = nullptr;
			data.SkillAsset->SkillAsset_BeginPlay(OwnerCharacter, &skill);

			if (skill == nullptr)
			{
				FLog::Log("Warning: ActiveSkill creation failed for PatternID: " + FString::FromInt(info.PatternID));
			}

			info.ActiveSkill = skill;
		}

		if (PatternInfos.Contains(info.PatternID))
		{
			FLog::Log("Previous Exist Data Try id : " + FString::FromInt(info.PatternID));
			continue;
		}
		PatternInfos.Add({ info.PatternID, info });
	}

	ACBoss_AI* boss = Cast<ACBoss_AI>(OwnerCharacter);
	if (!!boss)
		REGISTER_EVENT_WITH_REPLACE(boss, OnBossPhaseUpdated, this, UCPatternComponent::UpdatedPhase);
}


void UCPatternComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CheckFalse(PatternInfos.Num() > 0);

	for (auto& pair : PatternInfos)
	{
		if (pair.Value.ActiveSkill != nullptr)
			pair.Value.ActiveSkill->Update_Cooldown(DeltaTime);
	}
}

void UCPatternComponent::ExecutePattern()
{
	CheckTrue(bExecutePattern);
	CheckNull(CurrentSkill);

	bExecutePattern = true;

	//FLog::Print("Execute Pattern : " + CurrentSkill->GetName());
	CurrentSkill->ExecuteSkill();

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(state);

	state->SetActionMode();
}

void UCPatternComponent::DecidePattern()
{
	CheckTrue(bDecided);
	CheckTrue(bExecutePattern);

	TArray<int32> selectedIds;

	for (TPair<int32, FPatternInfo>& Pattern : PatternInfos)
	{
		if (Pattern.Value.ActiveSkill == nullptr)
			continue;

		if (Pattern.Value.Phase > CurrentPhase)
			continue;

		bool bCanExecute = true;
		bCanExecute &= Pattern.Value.ActiveSkill->IsCooldown();

		for (int32 ConditionID : Pattern.Value.ConditionIDs)
		{
			if (!!PatternCondition && PatternCondition->CheckCondition(ConditionID, OwnerCharacter) == false)
			{
				bCanExecute &= false;
				break;
			}
		}

		if (bCanExecute  == false && PatternDecider->EvaluatePattern() == EPatternDecision::KeepPattern)
			bCanExecute = true; 

		if (bCanExecute)
		{
			selectedIds.HeapPush(Pattern.Value.PatternID, [this](int32 a, int32 b)
				{
					return PatternInfos[a].Priority > PatternInfos[b].Priority;
				});
		}
	}

	if (selectedIds.Num() <= 0)
		return;

	bDecided = true;
	SelectedPatternID = selectedIds.HeapTop();

	// 현재 동작할 패턴 스킬 할당.
	if (PatternInfos.Contains(SelectedPatternID))
		CurrentSkill = PatternInfos[SelectedPatternID].ActiveSkill;

	if (PatternInfos.Contains(SelectedPatternID))
		DYNAMIC_EVENT_CALL_ONE_PARAM(OnDecidedPattern_Range, PatternInfos[SelectedPatternID].ActionRange);

	//FLog::Print("Decide Pattern : " + FString::FromInt(SelectedPatternID));
}






// -----------------------------------------------------------------------------------
/// <summary>
///  Pattern Out of Call
/// </summary>

void UCPatternComponent::Begin_Pattern()
{
	CheckNull(CurrentSkill);

	bExecutePattern = true;
	//FLog::Print("Begin_Pattern : " + (CurrentSkill->GetName()));
}

void UCPatternComponent::End_Pattern()
{
	CheckNull(CurrentSkill);

	//FLog::Print("End_Pattern : " + (CurrentSkill->GetName()));
	CurrentSkill->CompleteSkill();

	bDecided = false; // 패턴 결정권 다시 복구
	bExecutePattern = false;	// 수행한 패턴 정상 종료 플래그 ON
	CurrentSkill = nullptr; // 스킬을 전부 지움 
}

void UCPatternComponent::OnActivated_Collision(FName InName)
{
	CheckNull(CurrentSkill);

	//FLog::Print("OnActivated_Collision : " + CurrentSkill->GetName());
	CurrentSkill->OnActivated_Collision(InName);
}

void UCPatternComponent::OnDeactivated_Collision(FName InName)
{
	CheckNull(CurrentSkill);

	//FLog::Print("OnDeactivated_Collision : " + CurrentSkill->GetName());
	CurrentSkill->OnDeactivated_Collision(InName);
}

void UCPatternComponent::UpdatedPhase(int32 InPhase)
{
	CurrentPhase = InPhase;
	FLog::Log("Boss Current Phase : " + FString::FromInt(CurrentPhase));
}

