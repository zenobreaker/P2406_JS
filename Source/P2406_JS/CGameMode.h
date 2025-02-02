#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CGameMode.generated.h"

UCLASS()
class P2406_JS_API ACGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACGameMode();
	
public:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void SubscribeToSkillEvents(class UCUserWidget_SkillHUD* SkillHUD);

public:
	void OrderToAllActorDown();
	void ToggleEnemyUI();

private:
	UPROPERTY()
	class UCGameInstance* GameInstance;

private:
	bool bEnemyUiHide = true;
};
