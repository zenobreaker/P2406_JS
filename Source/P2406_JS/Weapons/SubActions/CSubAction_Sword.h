#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction.h"
#include "../CWeaponStructures.h"
#include "CSubAction_Sword.generated.h"


UCLASS()
class P2406_JS_API UCSubAction_Sword : public UCSubAction
{
	GENERATED_BODY()
	
public:
	UCSubAction_Sword();

public:
	void Pressed() override;
	void Released() override;

public:
	void End_DoSubAction() override;

public:
	void GetSubActionDatas(const TArray<FDoActionData>& InDoSubActionDatas);

	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther);

private:
	int32 Index; 

	TArray<FDoActionData> DoSubActionDatas;
	TArray<class ACharacter*> Hitted;
};
