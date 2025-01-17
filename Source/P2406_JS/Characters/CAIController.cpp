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

	// 시야 구성 
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = 600; // 시야 범위 
	Sight->LoseSightRadius = 800; // 시야 범위를 벗어나면 감지를 잃기 시작하는 범위 
	Sight->PeripheralVisionAngleDegrees = 45; // 감지 시야 좌우 각도 
	Sight->SetMaxAge(2); // 감지를 벗어나면 잃기 시작하는 시간 

	// 블루프린트에서 사용 불가능 
	Sight->DetectionByAffiliation.bDetectEnemies = true; // 적 감지 
	Sight->DetectionByAffiliation.bDetectNeutrals = true; // 중립 감지 
	Sight->DetectionByAffiliation.bDetectFriendlies = false; // 아군 감지 

	// 갖는 대상이 배열일 땐 객체로 전달한다.
	Perception->ConfigureSense(*Sight);
	Perception->SetDominantSense(*Sight->GetSenseImplementation());

	// 팀 구성
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

	// 스마트 포인터의 주소에 대한 레퍼런스 값 반환
	UBlackboardComponent* blackboard = Blackboard.Get();
	// Enemy에 Blackboard asset을 사용한다고 명시
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
	// 가장 가까운 유효한 타겟을 찾기
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

	// Blackboard에 유효한 타겟 설정
	Blackboard->SetValueAsObject("Target", validTarget);
}

void ACAIController::HandleTeamPerception(AActor* InActor)
{

}


void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	CheckNull(Perception);

	// nullptr은 다 감지하는 것을 의미
	TArray<AActor*> actors;
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);

	for (AActor* actor : actors)
	{
		if (actor == nullptr || !actor->IsValidLowLevelFast() || actor->IsPendingKill())
			continue; 

		// Sight 감각에 대해 처리
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



