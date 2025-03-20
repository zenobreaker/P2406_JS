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


	// ����Ʈ �������� �ּҿ� ���� ���۷��� �� ��ȯ
	UBlackboardComponent* blackboard = Blackboard.Get();
	// Enemy�� Blackboard asset�� ����Ѵٰ� ���
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

	FVector enemyLocation = Enemy->GetActorLocation();
	FVector targetLocation = InTarget->GetActorLocation();

	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(enemyLocation, targetLocation);
	FRotator currentRotation = Enemy->GetActorRotation();
	float deltaYaw = /*FMath::Abs*/(FMath::FindDeltaAngleDegrees(currentRotation.Yaw, targetRotation.Yaw));

	// ���� ���̿� ���� ���� �ӵ��� ���� ����
	float rotationSpeed = FMath::GetMappedRangeValueClamped(RotateAngleRange, RotateSpeedRange, FMath::Abs(deltaYaw));
	// ���� ������ ����Ͽ� ������ �ӵ��� ȸ��
	//float rotationDirection = deltaYaw < 0.0f ? 1.0f : -1.0f;
	//FRotator newRotation = FMath::Lerp(currentRotation, targetRotation, InDeltaTime * rotationSpeed);
	//float targetYaw = currentRotation.Yaw + rotationDirection * rotationSpeed * InDeltaTime;
	FLog::Print("Rotate Speed " + FString::SanitizeFloat(rotationSpeed) + " Yaw : " + FString::SanitizeFloat(deltaYaw), 36360);
	// ȸ�� ���� ����
	if (deltaYaw < 0.0f) // �ݽð���� ȸ��
	{
		Enemy->SetActorRotation(FRotator(currentRotation.Pitch, currentRotation.Yaw - rotationSpeed * InDeltaTime, currentRotation.Roll));
	}
	else // �ð���� ȸ��
	{
		Enemy->SetActorRotation(FRotator(currentRotation.Pitch, currentRotation.Yaw + rotationSpeed * InDeltaTime, currentRotation.Roll));
	}
	// ������ �ٶ󺸴� ���� (������)
	DrawDebugLine(GetWorld(), enemyLocation, enemyLocation + Enemy->GetActorForwardVector() * 200.0f, FColor::Red, false, 0.1f, 0, 2.0f);

	// ��ǥ ���� (�Ķ���)
	DrawDebugLine(GetWorld(), enemyLocation, targetLocation, FColor::Blue, false, 0.1f, 0, 2.0f);

}	
