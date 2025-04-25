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
	// BT �½�ũ ���� ���� �� ȣ��˴ϴ�.
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// Tick �߿� ȣ��Ǿ� �񵿱� �۾�(�̵�)�� ���¸� üũ�մϴ�.
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	// �½�ũ�� �Ϸ�(����/����/���)�� �� ȣ��˴ϴ�.
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	// �����Ϳ��� ��� ������ ǥ���� �� ���˴ϴ�.
	virtual FString GetStaticDescription() const override;

public:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool bDebugMode;

	// Ÿ�� ���͸� ��� �ִ� ������ Ű�Դϴ�. Object �Ǵ� Actor Ÿ���̾�� �մϴ�.
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector TargetKey;

	// �̵��� ��ǥ ��Ÿ�(Ÿ�����κ����� �Ÿ�)�� ��� �ִ� ������ Ű�Դϴ�. Float Ÿ���̾�� �մϴ�.
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector RangeKey;

	// ��ǥ ��ġ�� �����ߴٰ� ������ ��� ���� �Ÿ��Դϴ�.
	UPROPERTY(EditAnywhere, Category = Move)
	float AcceptanceRadius;

	UPROPERTY(EditAnywhere, Category = Move)
	float BufferRadius = 100.0f;


private:
	// ���� ���� �̵� ��û ID�� �����մϴ�.
	FAIRequestID MoveRequestID;
	FVector TargetLocation; 
};