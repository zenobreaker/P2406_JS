#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Melee.generated.h"


UCLASS()
class P2406_JS_API UCBTService_Melee : public UBTService
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 150;
	
	UPROPERTY(EditAnywhere, Category = "Action")
	FVector2D ActionDelay = FVector2D::TVector2(1.5f, 2.0f);

public:
	UCBTService_Melee();

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	void RadnomActionDelay();

private:
	bool Tick_CheckWait() const;
	bool Tick_CheckPatrol(const ACharacter* InTarget);
	bool Tick_CheckAttack(const ACharacter* InTarget);
	bool Tick_CheckGuard(const ACharacter* InTarget) const;
	bool Tick_CheckApproach(const ACharacter* InTarget);
private:
	float CurrentDelay;

	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCStateComponent* CachedState = nullptr;
	class IIGuardable* CachedGuardable = nullptr;
};
