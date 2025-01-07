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

	// �����忡�� ��ǥ ��ġ ��������
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
	QueryParams.AddIgnoredActor(ai); // �ڱ� �ڽ��� ����

	FHitResult HitResult;
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,                // �浹 ���
		start,                    // ���� ����
		InLocation,                      // �� ����
		FQuat::Identity,          // ȸ���� (�ʿ��ϸ� ����)
		ECC_Pawn,           // Ʈ���̽� ä��
		FCollisionShape::MakeCapsule(capsuleRadius, capsuleHalfHeight),
		QueryParams               // ���� �Ķ����

	);

	DrawDebugLine(
		GetWorld(),
		start,
		InLocation,
		FColor::Green,    // ���� ���ο� ���� ���� ����
		false,         // ���� ����
		2.0f,          // ���� �ð�
		0,
		2.0f           // �β�
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
