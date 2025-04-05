#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"


UCLASS(BlueprintType)
class P2406_JS_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCGameInstance();

public:
	virtual void Init() override;

public:
	UPROPERTY(EditAnywhere, Category = "SkillManager")
	TSubclassOf<class UCSkillManager> SkillManagerClass;

	UPROPERTY(EditAnywhere, Category = "BattleManager")
	TSubclassOf<class UCBattleManager> BattleManagerClass;

	UPROPERTY(EditAnywhere, Category = "GameManager")
	TSubclassOf<class UCPatternConditionManager> PatternConditionClass;

	UPROPERTY(EditAnywhere, Category = "GameManager")
	TSubclassOf<class UCGameManager> GameManagerClass;

	UPROPERTY(EditAnywhere, Category = "Buff")
	TSubclassOf<class UCBuffUIManager> BuffUIManagerClass;

public:
	class UCPatternConditionManager* PatternCondition;
	class UCSkillManager* SkillManager;
	class UCBattleManager* BattleManager;
	class UCGameManager* GameManager;
	class UCBuffUIManager* BuffUIManager; 

};
