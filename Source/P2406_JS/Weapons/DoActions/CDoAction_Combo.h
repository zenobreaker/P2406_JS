#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "../IComboable.h"
#include "CDoAction_Combo.generated.h"


UCLASS()
class P2406_JS_API UCDoAction_Combo 
	: public UCDoAction
	, public IIComboable
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() override { bEnable = true; }
	FORCEINLINE void DisableCombo() override { bEnable = false; }

	FORCEINLINE bool GetEnableCombo() const { return  bEnable; }
	FORCEINLINE void SetExist(bool InValue) { bExist = InValue; }


public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

public:
	void OnAttachmentEndCollision() override;

	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class AActor* InOther) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class AActor* InOther) override;

private:
	int32 Index;

	bool bEnable;
	bool bExist;
};
