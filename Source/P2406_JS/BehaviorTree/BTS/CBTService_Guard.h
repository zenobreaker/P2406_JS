#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Guard.generated.h"


UCLASS()
class P2406_JS_API UCBTService_Guard : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_Guard(); 

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void CalcParryTimer(); 

	void OnCounterState(); 

	void OnGuardState(); 


private:
	UPROPERTY(EditAnywhere, Category = "Guard")
	float MaxParryTimer = 10.0f; 

	UPROPERTY(EditAnywhere, Category = "Guard")
	float ParryRandomDiff = 0.0f; 

private:
	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCStateComponent* CachedState = nullptr;
	class UCGuardComponent* GuardComp = nullptr; 
	class UCConditionComponent* Condition = nullptr;
	class UBlackboardComponent* Blackboard = nullptr;

private:
	float CurrentParry = 0.0f;
};
