#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_SpawnEntity.generated.h"


UCLASS()
class P2406_JS_API UCBTTaskNode_SpawnEntity : public UBTTaskNode
{
	GENERATED_BODY()

private: 
	UPROPERTY(EditAnywhere, Category = "Entity")
	TSubclassOf<AActor> EntityClass; 

	UPROPERTY(EditAnywhere, Category = "Entity")
	FVector SpawnLocation; 

	UPROPERTY(EditAnywhere, Category = "Entity")
	FRotator SpawnRotation;

public:
	UCBTTaskNode_SpawnEntity();

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
//	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
