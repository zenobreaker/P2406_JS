#include "BehaviorTree/Decorator/CBTDecorator_CheckMoveValid.h"
#include "Global.h"
#include "Characters/CAIController.h"
#include "Characters/CEnemy_AI.h"

#include "BehaviorTree/BlackboardComponent.h"

#include "Components/CapsuleComponent.h"
#include "Components/CMovementComponent.h"

UCBTDecorator_CheckMoveValid::UCBTDecorator_CheckMoveValid()
{
	NodeName = "CheckMoveValid";
}

bool UCBTDecorator_CheckMoveValid::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	ACAIController* AIController = Cast< ACAIController>(OwnerComp.GetAIOwner());
	if (!AIController) return false;

	// 블랙보드에서 목표 위치 가져오기
	FVector TargetLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(TargetLocationKey.SelectedKeyName);

	return CheckPathValidity(*AIController, TargetLocation);
}

bool UCBTDecorator_CheckMoveValid::CheckPathValidity(ACAIController& InController, FVector InLocation) const
{
	ACEnemy_AI * ai = Cast<ACEnemy_AI>(InController.GetPawn());
	CheckNullResult(ai, false); 

	UCapsuleComponent* capsule = FHelpers::GetComponent <UCapsuleComponent>(ai);
	CheckNullResult(capsule, false); 

	float capsuleRadius = capsule->GetScaledCapsuleRadius();
	float capsuleHalfHeight = capsule->GetScaledCapsuleHalfHeight();


	FVector start = ai->GetActorLocation();
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(ai); // 자기 자신은 무시

	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,                // 충돌 결과
		start,                    // 시작 지점
		InLocation,                      // 끝 지점
		FQuat::Identity,          // 회전값 (필요하면 설정)
		ECC_Pawn,           // 트레이스 채널
		FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight),
		QueryParams               // 쿼리 파라미터

	);

	DrawDebugLine(
		GetWorld(),
		start,
		InLocation,
		FColor::Green,    // 적중 여부에 따라 색상 변경
		false,         // 지속 여부
		2.0f,          // 지속 시간
		0,
		2.0f           // 두께
	);


	if (bHit)
	{
		/*UCMovementComponent* move = FHelpers::GetComponent<UCMovementComponent>(ai);
		FLog::Log("Condition = Trace hit Obstable !! "); 
		if (move != nullptr)
		{
			move->EnableControlRotation();
		}*/
		return false; 
	}

	return true;
}
