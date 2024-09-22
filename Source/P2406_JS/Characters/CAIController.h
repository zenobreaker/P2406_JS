#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CAIController.generated.h"


UCLASS()
class P2406_JS_API ACAIController : public AAIController
{
	GENERATED_BODY()


private:
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;

public:
	ACAIController();

protected:
	virtual void BeginPlay() override; 


protected:
	virtual void OnPossess(APawn* InPawn) override; 
	virtual void OnUnPossess() override; 

private:
	UFUNCTION()
	void OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors);

private:
	class ACEnemy_AI* Enemy;
	class UCAIBehaviorComponent* Behavior;

	class UAISenseConfig_Sight* Sight; // 감지용 클래스 
};
