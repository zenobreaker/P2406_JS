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

	// 시야 구성 
	Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("Sight");
	Sight->SightRadius = SightRadius; // 시야 범위 
	Sight->LoseSightRadius = LoseSightRadius; // 시야 범위를 벗어나면 감지를 잃기 시작하는 범위 
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees; // 감지 시야 좌우 각도 
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
	
	Sight->SightRadius = SightRadius; // 시야 범위 
	Sight->LoseSightRadius = LoseSightRadius; // 시야 범위를 벗어나면 감지를 잃기 시작하는 범위 
	Sight->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees; // 감지 시야 좌우 각도 
	Sight->SetMaxAge(2); // 감지를 벗어나면 잃기 시작하는 시간 

	// 갖는 대상이 배열일 땐 객체로 전달한다.
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

	// : 비헤이비어 트리 중지
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

// 상태가 바뀌면 콜 
void ACAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	CheckNull(Perception);
	CheckNull(GetPawn());

	// nullptr은 다 감지하는 것을 의미
	TArray<AActor*> actors;
	//현 시점에서 감지된 애들만 리턴 - 감지 대상에서만 보이므로 감지 잃은건 없음
	Perception->GetCurrentlyPerceivedActors(nullptr, actors);
	AActor* nearestTarget = nullptr;
	float nearestDistance = FLT_MAX;

	for (AActor* actor : actors)
	{
		// 유효성 체크
		if (actor == nullptr || !actor->IsValidLowLevelFast() /*|| actor->IsPendingKill()*/)
			continue;

		ACEnemy_AI* otherAI = Cast<ACEnemy_AI>(actor);
		// 팀 구분 체크 추가
		if (otherAI && otherAI->GetTeamID() == Enemy->GetTeamID ())
			continue; // 아군이면 무시


		// 거리 계산
		float distance = FVector::Dist(GetPawn()->GetActorLocation(), actor->GetActorLocation());

		// 가장 가까운 타겟 찾기
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

	// 루프 끝난 후 Blackboard 설정
	//Blackboard->SetValueAsObject("Target", nearestTarget);
	Behavior->SetTarget(Cast<ACharacter>(nearestTarget));

	// 현재 감지 된게 없다면 이전에 감지 정보를 저장 
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



