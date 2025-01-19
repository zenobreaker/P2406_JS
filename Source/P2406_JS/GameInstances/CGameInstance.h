#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CGameInstance.generated.h"


UCLASS()
class P2406_JS_API UCGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCGameInstance();

public:
	virtual void Init() override;

public:
	UPROPERTY()
	class UCSkillManager* SkillManager;

	UPROPERTY()
	class UCBattleManager* BattleManager;
};
