#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_PatternPostDecision.generated.h"

// 패턴 수행 후 처리를 하는 노드
UCLASS()
class P2406_JS_API UCBTService_PatternPostDecision : public UBTService
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	float CloseRange = 150;

	UPROPERTY(EditAnywhere)
	int StafeRate = 60; 


public:
	UCBTService_PatternPostDecision();

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

private:
	class UBlackboardComponent* Blackboard = nullptr;

private:
	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCPatternComponent* CachedPattern = nullptr;

};
