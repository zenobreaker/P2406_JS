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

	ACEnemy_AI* ai = Cast<ACEnemy_AI>(GetOwner());
	CheckNull(ai);

	UCStateComponent* state = FHelpers::GetComponent<UCStateComponent>(ai);
	if (!!state)
	{
		state->OnStateTypeChanged.AddDynamic(this, &UCAIBehaviorComponent::OnStateChanged);
	}

	UCConditionComponent* condition = FHelpers::GetComponent<UCConditionComponent>(ai);
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

bool UCAIBehaviorComponent::IsHittedMode()
{
	return GetType() == EAIStateType::Hitted;
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

void UCAIBehaviorComponent::SetHittedMode()
{
	ChangeType(EAIStateType::Hitted);
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


void UCAIBehaviorComponent::ChangeType(EAIStateType InType)
{
	if (Blackboard == nullptr)
		return; 

	EAIStateType prevType = GetType();

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
	switch (InType)
	{
		case EConditionState::CONDITION_DOWNED:
		case EConditionState::CONDITION_AIRBORNE:
		bCanMove = true;
	}
}



ACharacter* UCAIBehaviorComponent::GetTarget()
{
	return Cast<ACharacter>(Blackboard->GetValueAsObject(TargetKey));
}