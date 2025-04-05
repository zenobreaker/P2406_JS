#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Buffs/CBuffStructures.h"
#include "CGameManager.generated.h"

UENUM(BlueprintType)
enum class EGameFlowState
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
	UPROPERTY(EditAnywhere, Category ="Manager")
	TSubclassOf<class UCStageManager> StageManagerClass;

	UPROPERTY(EditAnywhere, Category ="Manager")
	TSubclassOf<class UCBuffManager> BuffManagerClass; 

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bDrawDebug = false; 

public:
	UCGameManager();

public:
	void BeginPlay(UWorld* InWorld);
	void StartGame();
	void SetGameState(EGameFlowState InState);

private: 
	void HandleState();

	void HandleBuffSelect();
	void HandleCombatPerparation();
	void HandleBossPreparation(); 
	void HandleBossBattle();

private: 
	void SendSelctedBuffFlow();

public:
	UFUNCTION()
	void OnStageCleared();

	UFUNCTION()
	void OnStartBossStage();

	UFUNCTION()
	void OnBossStageCleared();

	UFUNCTION()
	void OnBossSpawned(class ACBoss_AI* Boss);

	UFUNCTION()
	void OnShowBuffList();

	UFUNCTION()
	void OnHideBuffList();

public:
	FOnBossSpawned_GM OnBossSpawned_GM;

private:
	class UCStageManager* StageManager;
	class UCBuffManager* BuffManager;
	class UWorld* MyWorld;
private:
	EGameFlowState CurrentState = EGameFlowState::Max;

	bool bIsGameJoin = false; 
	bool bCommbatComplete = false; 
	TArray<struct FStatBuff> Buffs;
};
