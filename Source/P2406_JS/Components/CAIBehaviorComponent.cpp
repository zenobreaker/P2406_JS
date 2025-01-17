#include "Components/CAIBehaviorComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Components/CStateComponent.h"
#include "Components/CConditionComponent.h"


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

void UCAIBehaviorComponent::SetAriborneMode()
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
	if (Blackboard == nullptr)
		return;


	FString curType = StaticEnum<EAIStateType>()->GetNameStringByValue((int64)InType);
	//FLog::Log(GetOwner()->GetName() + " " + curType);

	EAIStateType prevType = GetType();

	//TODO: 임시 코드 
	

	Blackboard->SetValueAsEnum(AIStateTypeKey, (uint8)InType);

	if (OnAIStateTypeChanged.IsBound())
		OnAIStateTypeChanged.Broadcast(prevType, InType);
}

void UCAIBehaviorComponent::OnStateChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Down:
		case EStateType::Airborne:
		case EStateType::Damaged:
		bCanMove = false;
		break;

		case EStateType::Guard:
		case EStateType::Idle:
		bCanMove = true;
		default:
		bCanMove = true;
		break;
	}
}

void UCAIBehaviorComponent::OnAddCondiitionType(EConditionState InType)
{
	FLog::Log("OnAddCondiitionType Call");

	switch (InType)
	{
		case EConditionState::CONDITION_DOWNED:
		case EConditionState::CONDITION_AIRBORNE:
		bCanMove = false;
		break;
	}
}

void UCAIBehaviorComponent::OnRemoveConditionType(EConditionState InType)
{
	FLog::Log("OnRemoveConditionType Call");

	switch (InType)
	{
		case EConditionState::CONDITION_DOWNED:
		case EConditionState::CONDITION_AIRBORNE:
		bCanMove = true;
	}
}

void UCAIBehaviorComponent::OnCharacterDead()
{
	FLog::Log(CachedAI->GetName() + "Dead Call");
	// 아아 죽었나이까?.. 
	SetDeadMode();
}

ACharacter* UCAIBehaviorComponent::GetTarget()
{
	uint8 myTeamID = -1;
	if (!!CachedAI)
		myTeamID = CachedAI->GetTeamID();

	uint8 targetID = -2;
	auto* targetAI = Cast<ACEnemy_AI>(Blackboard->GetValueAsObject(TargetKey));
	if (targetAI != nullptr)
	{
		targetID = targetAI->GetTeamID();
	}

	CheckTrueResult(myTeamID == targetID, nullptr);

	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}