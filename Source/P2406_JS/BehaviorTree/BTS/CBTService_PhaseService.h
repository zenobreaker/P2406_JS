#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_PhaseService.generated.h"

UCLASS()
class P2406_JS_API UCBTService_PhaseService : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_PhaseService();

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
