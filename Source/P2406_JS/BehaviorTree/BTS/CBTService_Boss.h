#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Boss.generated.h"


UCLASS()
class P2406_JS_API UCBTService_Boss : public UBTService
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 150;

public:
	UCBTService_Boss();

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;


private:
	class UBlackboardComponent* Blackboard = nullptr;

private:
	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class IIGuardable* CachedGuardable = nullptr;
	class UCStateComponent* CachedState = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCPatternComponent* CachedPattern = nullptr;

};
