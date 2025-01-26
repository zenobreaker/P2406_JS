#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Team.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CHealthPointComponent.h"

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

	// : �����̺�� Ʈ�� ����
	UBehaviorTreeComponent* BehaviorTreeComp = FindComponentByClass<UBehaviorTreeComponent>();
	if (BehaviorTreeComp)
	{
		BehaviorTreeComp->StopTree(EBTStopMode::Safe);
	}

}

void ACAIController::HandleSightPerception(AActor* InActor)
{
	
}

void ACAIController::HandleTeamPerception(AActor* InActor)
{
	FLog::Log("this calling team Percept");
}


void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	CheckNull(Perception);
	CheckNull(GetPawn());

	// nullptr�� �� �����ϴ� ���� �ǹ�
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	AActor* nearestTarget = nullptr;
	float nearestDistance = FLT_MAX;

	for (AActor* actor : actors)
	{
		// ��ȿ�� üũ
		if (actor == nullptr || !actor->IsValidLowLevelFast() /*|| actor->IsPendingKill()*/)
			continue;

		ACEnemy_AI* otherAI = Cast<ACEnemy_AI>(actor);
		// �� ���� üũ �߰�
		if (otherAI && otherAI->GetTeamID() == Enemy->GetTeamID ())
			continue; // �Ʊ��̸� ����


		// �Ÿ� ���
		float distance = FVector::Dist(GetPawn()->GetActorLocation(), actor->GetActorLocation());

		// ���� ����� Ÿ�� ã��
		if (distance < nearestDistance)
		{
			UCHealthPointComponent* otherHealth = FHelpers::GetComponent<UCHealthPointComponent>(actor);
			if (otherHealth && !otherHealth->IsDead())
			{
				nearestTarget = actor;
				nearestDistance = distance;
			}
		}
	}

	// ���� ���� �� Blackboard ����
	Blackboard->SetValueAsObject("Target", nearestTarget);
}

void ACAIController::OnEnemyDead()
{
	UnPossess();

	ClearFocus(EAIFocusPriority::Gameplay);

	Blackboard->SetValueAsObject("Target", nullptr);
}



