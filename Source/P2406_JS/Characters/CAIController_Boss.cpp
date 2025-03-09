#include "Characters/CAIController_Boss.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "Characters/CEnemy_AI.h"
#include "Components/CAIBehaviorComponent.h"

void ACAIController_Boss::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);


	// 스마트 포인터의 주소에 대한 레퍼런스 값 반환
	UBlackboardComponent* blackboard = Blackboard.Get();
	// Enemy에 Blackboard asset을 사용한다고 명시
	UseBlackboard(Enemy->GetBehaviorTree()->BlackboardAsset, blackboard);

	Behavior = FHelpers::GetComponent<UCAIBehaviorComponent>(Enemy);
	CheckNull(Behavior);
	Behavior->SetBlackboard(Blackboard);

	RunBehaviorTree(Enemy->GetBehaviorTree());
}

void ACAIController_Boss::RotateTowardsTarget(float InDeltaTime, ACharacter* InTarget)
{
	CheckNull(Enemy);
	CheckNull(InTarget);

	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), InTarget->GetActorLocation());


	FRotator currentRotation = Enemy->GetActorRotation();
	FRotator newRotation = FMath::RInterpTo(currentRotation, targetRotation, InDeltaTime, 5.0f);
	Enemy->SetActorRotation(FRotator(currentRotation.Pitch, newRotation.Yaw, currentRotation.Roll));
}	
