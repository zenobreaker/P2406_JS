#include "Components/CAIBehaviorComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CConditionComponent.h"
#include "Components/CPatternComponent.h"

#include "GameInstances/CGameInstance.h"
#include "GameInstances/CBattleManager.h"

UCAIBehaviorComponent::UCAIBehaviorComponent()
{
}


void UCAIBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedAI = Cast<ACEnemy_AI>(GetOwner());
	CheckNull(CachedAI);

	REGISTER_EVENT_WITH_REPLACE(CachedAI, OnCharacterDead, this, UCAIBehaviorComponent::OnCharacterDead);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(CachedAI);
	if (!!state)
	{
		state->OnStateTypeChanged.AddDynamic(this, &UCAIBehaviorComponent::OnStateChanged);
	}

	UCConditionComponent* condition = FHelpers::GetComponent<UCConditionComponent>(CachedAI);
	if (!!condition)
	{
		condition->OnAddCondiitionType.AddDynamic(this, &UCAIBehaviorComponent::OnAddCondiitionType);
		condition->OnRemoveCondiitionType.AddDynamic(this, &UCAIBehaviorComponent::OnRemoveConditionType);
	}

	UCPatternComponent* pattern = FHelpers::GetComponent<UCPatternComponent>(CachedAI);
	REGISTER_EVENT_WITH_REPLACE(pattern, OnDecidedPattern, this, UCAIBehaviorComponent::OnDecidedPattern);
	REGISTER_EVENT_WITH_REPLACE(pattern, OnDecidedPattern_Range, this, UCAIBehaviorComponent::OnDecidedPattern_Range);

	if(!!Blackboard)
		Blackboard->SetValueAsBool("bCanAct", true);
}

EAIStateType UCAIBehaviorComponent::GetType()
{
	return (EAIStateType)Blackboard->GetValueAsEnum(AIStateTypeKey);
}


FVector UCAIBehaviorComponent::GetPatrolLocation()
{
	return Blackboard->GetValueAsVector(PatrolLocationKey);
}


void UCAIBehaviorComponent::SetPatrolLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(PatrolLocationKey, InLocation);
}


FVector UCAIBehaviorComponent::GetEqsLocation()
{
	return Blackboard->GetValueAsVector(EqsLocationKey);
}

void UCAIBehaviorComponent::SetEqsLocation(const FVector& InLocation)
{
	Blackboard->SetValueAsVector(EqsLocationKey, InLocation);
}

bool UCAIBehaviorComponent::GetPattrenDecide()
{
	return Blackboard->GetValueAsBool(PatternDecideKey);
}

void UCAIBehaviorComponent::SetPatternDecide(bool InPatternDecide)
{
	Blackboard->SetValueAsBool(PatternDecideKey, InPatternDecide);
}

bool UCAIBehaviorComponent::GetPatternExecute()
{
	return Blackboard->GetValueAsBool(PatternExecuteKey);
}

void UCAIBehaviorComponent::SetPatternExecute(bool InValue)
{
	Blackboard->SetValueAsBool(PatternExecuteKey, InValue);
}

float UCAIBehaviorComponent::GetActionRange()
{
	return Blackboard->GetValueAsFloat(ActionRangeKey);
}

void UCAIBehaviorComponent::SetActionRange(float InActionRange)
{
	Blackboard->SetValueAsFloat(ActionRangeKey, InActionRange);
}


bool UCAIBehaviorComponent::IsWaitMode()
{
	return GetType() == EAIStateType::Wait;
}

bool UCAIBehaviorComponent::IsApproachMode()
{
	return GetType() == EAIStateType::Approach;
}

bool UCAIBehaviorComponent::IsActionMode()
{
	return GetType() == EAIStateType::Action;
}

bool UCAIBehaviorComponent::IsPatrolMode()
{
	return GetType() == EAIStateType::Patrol;
}

bool UCAIBehaviorComponent::IsDamageMode()
{
	return GetType() == EAIStateType::Damage;
}

bool UCAIBehaviorComponent::IsAvoidMode()
{
	return GetType() == EAIStateType::Avoid;
}

bool UCAIBehaviorComponent::IsAirborneMode()
{
	return GetType() == EAIStateType::Airborne;
}

bool UCAIBehaviorComponent::IsDownMode()
{
	return GetType() == EAIStateType::Down;
}

bool UCAIBehaviorComponent::IsGuardMode()
{
	return GetType() == EAIStateType::Guard;
}

bool UCAIBehaviorComponent::IsDeadMode()
{
	return GetType() == EAIStateType::Dead;
}

void UCAIBehaviorComponent::SetWaitMode()
{
	ChangeType(EAIStateType::Wait);
}

void UCAIBehaviorComponent::SetApproachMode()
{
	ChangeType(EAIStateType::Approach);
}

void UCAIBehaviorComponent::SetActionMode()
{
	ChangeType(EAIStateType::Action);
}

void UCAIBehaviorComponent::SetPatrolMode()
{
	ChangeType(EAIStateType::Patrol);
}

void UCAIBehaviorComponent::SetDamageMode()
{
	ChangeType(EAIStateType::Damage);
}

void UCAIBehaviorComponent::SetAvoidMode()
{
	ChangeType(EAIStateType::Avoid);
}

void UCAIBehaviorComponent::SetAirborneMode()
{
	ChangeType(EAIStateType::Airborne);
}

void UCAIBehaviorComponent::SetDownMode()
{
	ChangeType(EAIStateType::Down);
}

void UCAIBehaviorComponent::SetGuardMode()
{
	ChangeType(EAIStateType::Guard);
}

void UCAIBehaviorComponent::SetDeadMode()
{
	ChangeType(EAIStateType::Dead);
}

void UCAIBehaviorComponent::SetNoneMode()
{
	ChangeType(EAIStateType::Max);
}

void UCAIBehaviorComponent::SetPatternDecideMode()
{
	ChangeType(EAIStateType::Pattern_Decide);
}

FString UCAIBehaviorComponent::EnumToString(EAIStateType InType)
{
	static const TMap<EAIStateType, FString> EnumToStringMap =
	{
		{EAIStateType::Wait, TEXT("Wait")},
		{EAIStateType::Approach, TEXT("Approach")},
		{EAIStateType::Patrol, TEXT("Approach")},
		{EAIStateType::Action, TEXT("Action")},
		{EAIStateType::Dead, TEXT("Dead")},
		{EAIStateType::Guard, TEXT("Guard")},
		{EAIStateType::Max, TEXT("Max")}
	};

	if (const FString* Result = EnumToStringMap.Find(InType))
	{
		return *Result;
	}

	return TEXT("Unknown");
}


void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	CheckNull(Blackboard);


	FString curType = StaticEnum<EAIStateType>()->GetNameStringByValue((int64)InType);
	//FLog::Print(GetOwner()->GetName() + " " + curType + " " );

	PrevType = GetType();

	//TODO: 임시 코드 
	if (PrevType == EAIStateType::Dead)
		return;
	// 과거와 현재가 같다면 그것은 수행하지 않음. 
	if (PrevType == InType)
		return; 

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);
	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(PrevType, InType);

	bool bCheck = false;

	switch (InType)
	{
	case EAIStateType::Wait:
		break;
	case EAIStateType::Approach:
		break;
	case EAIStateType::Action:
		bCheck |= Blackboard->GetValueAsBool("bFirstAttack");
		break;
	case EAIStateType::Patrol:
		break;
	case EAIStateType::Damage:
		bCheck |= Blackboard->GetValueAsBool("bFirstDamage");
		break;
	case EAIStateType::Avoid:
		break;
	case EAIStateType::Airborne:
		break;
	case EAIStateType::Down:
		break;
	case EAIStateType::Guard:
		break;
	case EAIStateType::Dead:
		break;
	case EAIStateType::Max:
		break;
	default:
		break;
	}
	
	
	if (bCheck)
	{

		UCGameInstance* instance = Cast<UCGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
		CheckNull(instance);
		UCBattleManager* battleManager = instance->BattleManager;
		CheckNull(battleManager);

		auto* target = GetTarget();
		CheckNull(target);
		battleManager->RequestBattleParticipation(CachedAI->GetGroupID(), CachedAI, target);
	}
}

void UCAIBehaviorComponent::OnStateChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Down:
		case EStateType::Airborne:
		case EStateType::Damaged:

		break;

		case EStateType::Guard:
		case EStateType::Idle:
		break; 
	}
}

void UCAIBehaviorComponent::OnAddCondiitionType(EConditionState InType)
{
	FLog::Log("OnAddCondiitionType Call");
	CheckNull(Blackboard); 

	switch (InType)
	{
		case EConditionState::CONDITION_DOWNED:
		case EConditionState::CONDITION_AIRBORNE:
		Blackboard->SetValueAsBool("bCanAct", false);
		break;
	}
}

void UCAIBehaviorComponent::OnRemoveConditionType(EConditionState InType)
{
	FLog::Log("OnRemoveConditionType Call");
	CheckNull(Blackboard);

	switch (InType)
	{
		case EConditionState::CONDITION_DOWNED:
		case EConditionState::CONDITION_AIRBORNE:
		//bCanMove = false;
		Blackboard->SetValueAsBool("bCanAct", true);
		break; 
	}
}

void UCAIBehaviorComponent::OnCharacterDead()
{
	FLog::Log(CachedAI->GetName() + "Dead Call");
	// 아아 죽었나이까?.. 
	SetDeadMode();
}

void UCAIBehaviorComponent::OnDecidedPattern(bool InValue)
{
	SetPatternDecide(InValue);
}

void UCAIBehaviorComponent::OnDecidedPattern_Range(float InValue)
{
	Blackboard->SetValueAsFloat(ActionRangeKey, InValue);
}

ACharacter* UCAIBehaviorComponent::GetTarget()
{
	CheckNullResult(CachedAI, nullptr);
	CheckNullResult(Blackboard, nullptr);

	int32 myTeamID = -1;
	myTeamID = CachedAI->GetTeamID();

	int32 targetID = -2;
	ACEnemy_AI* targetAI = Cast<ACEnemy_AI>(Blackboard->GetValueAsObject(TargetKey));
	if(targetAI != nullptr )
	{
		targetID = targetAI->GetTeamID();
		CheckTrueResult(myTeamID == targetID, nullptr);
	}

	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}

void UCAIBehaviorComponent::SetTarget(ACharacter* InTarget)
{
	CheckNull(InTarget);

	Blackboard->SetValueAsObject(TargetKey, InTarget);
}
