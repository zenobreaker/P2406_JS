#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_CheckRotate.generated.h"


UCLASS()
class P2406_JS_API UCBTService_CheckRotate : public UBTService
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, Category = "Rotate")
	FName IsRotatingKey = "bIsRotating";

	UPROPERTY(EditAnywhere, Category = "Rotate")
	FName DesiredRotationKey = "DesiredRotation";

	UPROPERTY(EditAnywhere, Category = "Rotate")
	FName IsLeftRotateKey = "bIsLeftRotate";

public: 
	UCBTService_CheckRotate();

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	class UBlackboardComponent* Blackboard = nullptr;

private:
	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCPatternComponent* CachedPattern = nullptr;

};
