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

	FVector enemyLocation = Enemy->GetActorLocation();
	FVector targetLocation = InTarget->GetActorLocation();

	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(enemyLocation, targetLocation);
	FRotator currentRotation = Enemy->GetActorRotation();
	float deltaYaw = /*FMath::Abs*/(FMath::FindDeltaAngleDegrees(currentRotation.Yaw, targetRotation.Yaw));

	// 각도 차이에 따라 보간 속도를 동적 조절
	float rotationSpeed = FMath::GetMappedRangeValueClamped(RotateAngleRange, RotateSpeedRange, FMath::Abs(deltaYaw));
	// 선형 보간을 사용하여 일정한 속도로 회전
	//float rotationDirection = deltaYaw < 0.0f ? 1.0f : -1.0f;
	//FRotator newRotation = FMath::Lerp(currentRotation, targetRotation, InDeltaTime * rotationSpeed);
	//float targetYaw = currentRotation.Yaw + rotationDirection * rotationSpeed * InDeltaTime;
	FLog::Print("Rotate Speed " + FString::SanitizeFloat(rotationSpeed) + " Yaw : " + FString::SanitizeFloat(deltaYaw), 36360);
	// 회전 방향 결정
	if (deltaYaw < 0.0f) // 반시계방향 회전
	{
		Enemy->SetActorRotation(FRotator(currentRotation.Pitch, currentRotation.Yaw - rotationSpeed * InDeltaTime, currentRotation.Roll));
	}
	else // 시계방향 회전
	{
		Enemy->SetActorRotation(FRotator(currentRotation.Pitch, currentRotation.Yaw + rotationSpeed * InDeltaTime, currentRotation.Roll));
	}
	// 보스가 바라보는 방향 (빨간색)
	DrawDebugLine(GetWorld(), enemyLocation, enemyLocation + Enemy->GetActorForwardVector() * 200.0f, FColor::Red, false, 0.1f, 0, 2.0f);

	// 목표 방향 (파란색)
	DrawDebugLine(GetWorld(), enemyLocation, targetLocation, FColor::Blue, false, 0.1f, 0, 2.0f);

}	
