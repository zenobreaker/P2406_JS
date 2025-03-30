#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CGameManager.generated.h"

UENUM(BlueprintType)
enum class EGameState
{
	Start, 
	BuffSelect,
	CombatPreparation, 
	Combat, 
	BossPreparation, 
	BossBattle,
	End, 
	Max, 
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossSpawned_GM, class ACBoss_AI*, Boss);	

UCLASS(Blueprintable)
class P2406_JS_API UCGameManager : public UObject
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCStageManager> StageManagerClass;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug = false; 

public:
	UCGameManager();

public:
	void BeginPlay();
	void StartGame();
	void SetGameState(EGameState InState); 

private: 
	void HandleState();

	void HandleCombatPerparation();
	void HandleBossPreparation(); 
	void HandleBossBattle();

public:
	UFUNCTION()
	void OnStageCleared();

	UFUNCTION()
	void OnStartBossStage();

	UFUNCTION()
	void OnBossStageCleared();

	UFUNCTION()
	void OnBossSpawned(class ACBoss_AI* Boss);

public:
	FOnBossSpawned_GM OnBossSpawned_GM;

private:
	class UCStageManager* StageManager;

private:
	EGameState CurrentState = EGameState::Max;

	bool bIsGameJoin = false; 
	bool bCommbatComplete = false; 
};
