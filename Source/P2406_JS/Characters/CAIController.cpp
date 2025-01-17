#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Team.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "EnvironmentQuery/EnvQueryManager.h"

ACAIController::ACAIController()
{
	FHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");

	FHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");

	// �þ� ���� 
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600; // �þ� ���� 
	Sight->LoseSightRadius = 800; // �þ� ������ ����� ������ �ұ� �����ϴ� ���� 
	Sight->PeripheralVisionAngleDegrees = 45; // ���� �þ� �¿� ���� 
	Sight->SetMaxAge(2); // ������ ����� �ұ� �����ϴ� �ð� 

	// �������Ʈ���� ��� �Ұ��� 
	Sight->DetectionByAffiliation.bDetectEnemies = true; // �� ���� 
	Sight->DetectionByAffiliation.bDetectNeutrals = true; // �߸� ���� 
	Sight->DetectionByAffiliation.bDetectFriendlies = false; // �Ʊ� ���� 

	// ���� ����� �迭�� �� ��ü�� �����Ѵ�.
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

	// �� ����
	TeamConfig = CreateDefaultSubobject<UAISenseConfig_Team>("Team");
	Perception->ConfigureSense(*TeamConfig);
}

void ACAIController::BeginPlay()
{
	Super::BeginPlay();

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(Enemy->GetTeamID());

	CheckNull(Enemy->GetBehaviorTree());

	REGISTER_EVENT_WITH_REPLACE(Enemy, OnCharacterDead, this,
		ACAIController::OnEnemyDead);

	// ����Ʈ �������� �ּҿ� ���� ���۷��� �� ��ȯ
	UBlackboardComponent* blackboard = Blackboard.Get();
	// Enemy�� Blackboard asset�� ����Ѵٰ� ���
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, blackboard);

	Behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	CheckNull(Behavior);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController::OnUnPossess()
{
	Super::OnUnPossess();

}

void ACAIController::HandleSightPerception(AActor* InActor)
{
	// ���� ����� ��ȿ�� Ÿ���� ã��
	AActor* validTarget = nullptr;
	bool bCheck = true;

	UCAIBehaviorComponent* otherBehavior = FHelpers::GetComponent <UCAIBehaviorComponent>(InActor);
	if (!!otherBehavior)
	{
		bCheck &= otherBehavior->IsDeadMode() == false;
	}

	bCheck &= IsValid(InActor);
	bCheck &= InActor->IsPendingKill() == false;

	if (bCheck)
	{
		validTarget = InActor;
	}

	// Blackboard�� ��ȿ�� Ÿ�� ����
	Blackboard->SetValueAsObject("Target", validTarget);
}

void ACAIController::HandleTeamPerception(AActor* InActor)
{

}


void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	CheckNull(Perception);

	// nullptr�� �� �����ϴ� ���� �ǹ�
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	for (AActor* actor : actors)
	{
		if (actor == nullptr || !actor->IsValidLowLevelFast() || actor->IsPendingKill())
			continue; 

		// Sight ������ ���� ó��
		if (Sight != nullptr && 
			Perception->HasActiveStimulus(*actor, Sight->GetSenseID()))
		{
			HandleSightPerception(actor);
		}

		if (TeamConfig != nullptr && 
			Perception->HasActiveStimulus(*actor, TeamConfig->GetSenseID()))
		{
			HandleTeamPerception(actor);
		}
	}


	//TArray<AActor*> sightActors;
	//Perception->GetCurrentlyPerceivedActors(UAISenseConfig_Sight::StaticClass(), sightActors);

	/*TArray<AActor*> teamActors;
	Perception->GetCurrentlyPerceivedActors(TeamConfig->StaticClass(), teamActors);
	if (teamActors.Num() > 0)
		FLog::Print(Enemy->GetName() + "Team count " + FString::FromInt(teamActors.Num()), 1985 + Enemy->GetAIID());*/


	//for (AActor* actor : sightActors)
	//{
	//	HandleSightPerception(actor);
	//}
	//
	//for (AActor* actor : teamActors)
	//{
	//	HandleTeamPerception(actor);
	//}
}

void ACAIController::OnEnemyDead()
{
	ClearFocus(EAIFocusPriority::Gameplay);

	Blackboard->SetValueAsObject("Target", nullptr);
}



