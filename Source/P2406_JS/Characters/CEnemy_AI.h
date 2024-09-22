#pragma once

#include "CoreMinimal.h"
#include "Characters/CEnemy.h"
#include "CEnemy_AI.generated.h"

UCLASS()
class P2406_JS_API ACEnemy_AI : public ACEnemy
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	uint8 TeamID = 2;
	
	UPROPERTY(EditDefaultsOnly, Category = "Team")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	class ACPatrolPath* PatrolPath;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Label")
	float MaxLabelDistance = 1000.0f; 

private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent* Weapon; 

	UPROPERTY(VisibleAnywhere)
	class UCAIBehaviorComponent* Behavior;

private:
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* LabelWidget;

public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }

	FORCEINLINE class UBehaviorTree* GetBehaviorTree() { return BehaviorTree; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }
public:
	ACEnemy_AI();

protected:
	virtual void BeginPlay() override; 

public:
	virtual void Tick(float DeltaTime) override; 


private:
	UFUNCTION()
	void OnHealthPointChanged(float InHealth, float InMaxHealth);

private:
	void Tick_LabelRenderScale();

protected:
	void Damaged() override; 

public:
	void End_Damaged() override; 
};
