#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_MeleeCheck.generated.h"

UCLASS()
class P2406_JS_API UCBTService_MeleeCheck : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category= "Melee")
	float DetectionRadius = 150.0f;

public:
	UCBTService_MeleeCheck(); 


protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCStateComponent* CachedState = nullptr;
	class IIGuardable* CachedGuardable = nullptr;
};
