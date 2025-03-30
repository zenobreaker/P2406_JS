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

	UPROPERTY()
	class UCPatternConditionManager* PatternCondition;

	UPROPERTY(EditAnywhere, Category = "GameManager")
	TSubclassOf<class UCGameManager> GameManagerClass;

public:
	class UCSkillManager* SkillManager;
	class UCBattleManager* BattleManager;
	class UCGameManager* GameManager;
};
