#include "Characters/CAIController.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Characters/CEnemy_AI.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Team.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CHealthPointComponent.h"
#include "Weapons/Attachments/CAttachment_Destructible.h"

ACAIController::ACAIController()
{
	FHelpers::CreateActorComponent<UAIPerceptionComponent>(this, &Perception, "Perception");
	FHelpers::CreateActorComponent<UBlackboardComponent>(this, &Blackboard, "Blackboard");

	// �þ� ���� 
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = SightRadius; // �þ� ���� 
	Sight->LoseSightRadius = LoseSightRadius; // �þ� ������ ����� ������ �ұ� �����ϴ� ���� 
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees; // ���� �þ� �¿� ���� 
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
	
	Sight->SightRadius = SightRadius; // �þ� ���� 
	Sight->LoseSightRadius = LoseSightRadius; // �þ� ������ ����� ������ �ұ� �����ϴ� ���� 
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees; // ���� �þ� �¿� ���� 
	Sight->SetMaxAge(2); // ������ ����� �ұ� �����ϴ� �ð� 

	// ���� ����� �迭�� �� ��ü�� �����Ѵ�.
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

	Perception->OnPerceptionUpdated.AddDynamic(this, &ACAIController::OnPerceptionUpdated);
}

void ACAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Enemy = Cast<ACEnemy_AI>(InPawn);
	SetGenericTeamId(Enemy->GetTeamID());

	CheckNull(Enemy->GetBehaviorTree());

	REGISTER_EVENT_WITH_REPLACE(
		Enemy, 
		OnCharacterDead, 
		this,
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
	if (!!BehaviorTreeComp)
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

void ACAIController::RegsterDestroyedEntity(AActor* InActor)
{
	ACAttachment_Destructible* destructible = Cast<ACAttachment_Destructible>(InActor); 
	CheckNull(destructible); 

	REGISTER_EVENT_WITH_REPLACE(destructible, OnDestroyedObject, this, ACAIController::OnDestroyedEntity);
}

// ���°� �ٲ�� �� 
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	CheckNull(Perception);
	CheckNull(GetPawn());

	// nullptr�� �� �����ϴ� ���� �ǹ�
	TArray<AActor*> actors;
	//�� �������� ������ �ֵ鸸 ���� - ���� ��󿡼��� ���̹Ƿ� ���� ������ ����
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
	//Blackboard->SetValueAsObject("Target", nearestTarget);
	Behavior->SetTarget(Cast<ACharacter>(nearestTarget));

	// ���� ���� �Ȱ� ���ٸ� ������ ���� ������ ���� 
	if (actors.IsEmpty())
	{
		auto target = Behavior->GetTarget(); 
		Behavior->SetLateTarget(target);
	}
}

void ACAIController::OnEnemyDead()
{
	UnPossess();

	ClearFocus(EAIFocusPriority::Gameplay);

	//Blackboard->SetValueAsObject("Target", nullptr);
	
	Behavior->SetLateTarget(nullptr);
	Behavior->SetTarget(nullptr);
}

void ACAIController::OnDestroyedEntity()
{
	FLog::Log("Destroyed Object");
	Blackboard->SetValueAsObject("PickupObject", nullptr); 
	Blackboard->SetValueAsBool("bIsOpeningPattern", false);
}



