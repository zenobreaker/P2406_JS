#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTask_MoveToTargetRange.generated.h"


UCLASS()
class P2406_JS_API UCBTTask_MoveToTargetRange : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTask_MoveToTargetRange(); 


protected:
	// BT 태스크 실행 시작 시 호출됩니다.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Tick 중에 호출되어 비동기 작업(이동)의 상태를 체크합니다.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// 태스크가 완료(성공/실패/취소)될 때 호출됩니다.
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	// 에디터에서 노드 설명을 표시할 때 사용됩니다.
	virtual FString GetStaticDescription() const override;

public:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool bDebugMode;

	// 타겟 액터를 담고 있는 블랙보드 키입니다. Object 또는 Actor 타입이어야 합니다.
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetKey;

	// 이동할 목표 사거리(타겟으로부터의 거리)를 담고 있는 블랙보드 키입니다. Float 타입이어야 합니다.
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector RangeKey;

	// 목표 위치에 도달했다고 간주할 허용 오차 거리입니다.
	UPROPERTY(EditAnywhere, Category = Move)
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere, Category = Move)
	float BufferRadius = 100.0f;


private:
	// 진행 중인 이동 요청 ID를 저장합니다.
	FAIRequestID MoveRequestID;
	FVector TargetLocation; 
};