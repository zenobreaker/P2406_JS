#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_FocusOnTarget.generated.h"


UCLASS()
class P2406_JS_API UCBTService_FocusOnTarget : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_FocusOnTarget();

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override; 

	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
private:
	class ACAIController * CachedController = nullptr;
	class ACEnemy_AI  * CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
};
