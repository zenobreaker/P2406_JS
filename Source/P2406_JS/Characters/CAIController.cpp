#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Components/CAIBehaviorComponent.h"
#include "Perception/AISenseConfig_Sight.h"
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

void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	TArray<AActor*> actors;
	// nullptr�� �� �����ϴ� ���� �ǹ�
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	//for (AActor* actor : actors)
		//CLog::Print(actor->GetName());

	if (actors.Num() > 0)
	{
		Blackboard->SetValueAsObject("Target", actors[0]);

		return;
	}

	Blackboard->SetValueAsObject("Target", nullptr);
}

