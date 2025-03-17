#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_PatternSelector.generated.h"


// 패턴 의사 결정 서비스 노드 
UCLASS()
class P2406_JS_API UCBTService_PatternSelector : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_PatternSelector(); 

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class UBlackboardComponent* Blackboard = nullptr;

private:
	class ACAIController* CachedController = nullptr;
	class ACBoss_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCPatternComponent* CachedPattern = nullptr;
};
