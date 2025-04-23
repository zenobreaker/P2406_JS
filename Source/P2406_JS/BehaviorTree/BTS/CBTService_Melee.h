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
	float WaitRange = 250;
	
	UPROPERTY(EditAnywhere, Category = "Action")
	FVector2D ActionDelay = FVector2D::TVector2(1.5f, 2.0f);

public:
	UCBTService_Melee();

protected:
	void OnSearchStart(FBehaviorTreeSearchData& SearchData) override;
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	void RadnomActionDelay();

private:
	void SetFocus(class ACharacter* InTarget) const; 

	void Calc_DelayTimer(float DeltaSeconds);

private:
	
	bool Notify_Battle_IsContainAttacker(class ACharacter* InTarget);
	void Notify_Battle_FindBattle(class ACharacter** OutTarget);
	//void Notify_Battle_JoinBattle(int32 InGroupID, class ACEnemy_AI* Initiator, class ACharacter* InTarget);
	void Notify_Battle_RemoveBattle(class ACharacter* InTarget, class ACEnemy_AI* Initiator);
	void Notify_Battle_SetJoinBattle(class ACharacter* InTarget, class ACEnemy_AI* Initiator);

private:
	bool IsTargetAttackable(class ACharacter* InTarget);

private:
	float CurrentDelay = 2.0f;

	class ACAIController* CachedController = nullptr;
	class ACEnemy_AI* CachedAI = nullptr;
	class UCAIBehaviorComponent* CachedBehavior = nullptr;
	class UCStateComponent* CachedState = nullptr;
	class UCConditionComponent* Condition= nullptr;
	class IIGuardable* CachedGuardable = nullptr;
	class UBlackboardComponent* Blackboard = nullptr;

private:
	class ACharacter* PrevTarget = nullptr;
	bool isFirstAttack = false;
	bool isFirstDamage = false; 

};
