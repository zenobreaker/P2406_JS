#include "BehaviorTree/BTS/CBTService_CheckRotate.h"
#include "Global.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CAIController.h"
#include "Characters/CBoss_AI.h"
#include "Characters/CEnemy_AI.h"

#include "Components/CAIBehaviorComponent.h"
#include "Components/CPatternComponent.h"

UCBTService_CheckRotate::UCBTService_CheckRotate()
{
	NodeName = "Check Rotate";

	Interval = 0.1f;
	RandomDeviation = .0f;
	//	bTickIntervals = true;
}

void UCBTService_CheckRotate::OnSearchStart(FBehaviorTreeSearchData& SearchData)
{
	Super::OnSearchStart(SearchData);

	CachedController = Cast<ACAIController>(SearchData.OwnerComp.GetOwner());
	if (!!CachedController)
	{
		CachedAI = Cast<ACEnemy_AI>(CachedController->GetPawn());
	}

	if (!!CachedAI)
	{
		CachedBehavior = FHelpers::GetComponent<UCAIBehaviorComponent>(CachedAI);
		if (!!CachedBehavior)
		{
			Blackboard = CachedBehavior->GetBlackboard();
		}

		CachedPattern = FHelpers::GetComponent<UCPatternComponent>(CachedAI);
	}
}

void UCBTService_CheckRotate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CheckNull(CachedAI);
	CheckNull(CachedBehavior);

	Blackboard = CachedBehavior->GetBlackboard();

	CheckNull(Blackboard);

	//TODO: 하드코딩 수정해보기
	bool open = Blackboard->GetValueAsBool("bIsOpeningPattern");
	if (open == true)
		return; 

	ACharacter* target = CachedBehavior->GetTarget();
	ACharacter* lateTarget = CachedBehavior->GetLateTarget();
	if (target == nullptr && lateTarget == nullptr)
		return;

	// 내 시야에 없는지 검사 
	FVector forward = CachedAI->GetActorForwardVector();
	FVector toTarget = (target->GetActorLocation() - CachedAI->GetActorLocation()).GetSafeNormal2D();

	float dot = FVector::DotProduct(forward, toTarget);
	float angle = FMath::RadiansToDegrees(FMath::Acos(dot));

	//TODO : Sight 값을 별도로 주는 게 좋을 듯 
	float sight = CachedController->GetPeripheralVisionAngleDegrees();
	
	// Sight 각이 감지 값이랑 같으면 회전하다가 감지되면 스위치가 꺼지고 관련된 이벤트가 종료된다.
	if (angle > sight) // 내 시야각 보다 크다면 회전이 필요하다 
	{
		FVector cross = FVector::CrossProduct(forward, toTarget);
		float direction = cross.Z;

		Blackboard->SetValueAsBool(IsLeftRotateKey, direction > 0);
		CachedBehavior->SetRotating(true); 
		double yaw = 0.0;
		if (target != nullptr)
			yaw = target->GetActorRotation().Yaw;
		else if (lateTarget != nullptr)
			yaw = lateTarget->GetActorRotation().Yaw;

		CachedController->ClearFocus(EAIFocusPriority::Gameplay);
		Blackboard->SetValueAsFloat(DesiredRotationKey, yaw);
	}
	else
	{
		CachedBehavior->SetRotating(false);
	}
}
