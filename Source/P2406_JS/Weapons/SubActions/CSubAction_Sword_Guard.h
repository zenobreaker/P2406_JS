#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "CSubAction_Sword_Guard.generated.h"


UCLASS(Blueprintable)
class P2406_JS_API UCSubAction_Sword_Guard : public UCSubAction
{
	GENERATED_BODY()

public:
	UCSubAction_Sword_Guard();

	UPROPERTY(EditAnywhere, Category = "Action")
	class UAnimMontage* GuardMontage;

	UPROPERTY(EditAnywhere, Category = "Action")
	class UAnimMontage* GuardHitMontage;

public:
	void Pressed() override;

private:
	void OnPressSpecialAction();

public:
	void Begin_DoSubAction_Implementation() override;
	void End_DoSubAction_Implementation() override;

	void Evaluate_GuradStance();

private:

	FTimerHandle GuardTimer;
	float GuardHP;
};
