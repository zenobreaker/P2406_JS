#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "Components/TimelineComponent.h"
#include "Enums/FEnum.h"
#include "CSubAction_Sword_Guard.generated.h"

UCLASS(Blueprintable)
class P2406_JS_API UCSubAction_Sword_Guard : public UCSubAction
{
	GENERATED_BODY()

public:
	UCSubAction_Sword_Guard();


public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;

private:
	void OffGuardStance();
public:
	void Pressed() override;
	void Released() override;

public:
	virtual bool TryGuard(struct ACBaseCharacter::FDamageData& DamageData) override;

public:
	UFUNCTION()
	void End_CounterAttack();


private:
	class UCGuardComponent* Guard; 

};
