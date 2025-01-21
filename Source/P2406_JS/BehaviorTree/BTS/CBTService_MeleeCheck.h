#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_MeleeCheck.generated.h"

UCLASS()
class P2406_JS_API UCBTService_MeleeCheck : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Melee")
	float DetectionRadius = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Melee")
	FVector2D ThinkValue = FVector2D(0.0f, 1.0f);

	UPROPERTY(EditAnywhere, Category = "Melee")
	FVector2D WeightValue = FVector2D(0.0f, 1.0f);

public:
	UCBTService_MeleeCheck();


protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool Calc_ThinkValue();
	
	// 공격이나 추격을 못하게 하기 위한 행위를 어떤 것으로 할 것인지 결정하는 함수
	void Calc_WaitAndAvoidWithWeight();

	void ChangeTarget(class ACharacter* InTarget);

private:
	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCStateComponent* CachedState = nullptr;
	class IIGuardable* CachedGuardable = nullptr;

private:
	class ACharacter* PrevTarget = nullptr; 
};
