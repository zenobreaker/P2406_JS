#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Characters/CBaseCharacter.h"
#include "CSubAction.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSAGuardValueChanged, float, InValue,
	float, InMaxValue);

UCLASS()
class P2406_JS_API UCSubAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }
	FORCEINLINE bool GetInSpecialAction() { return bInSpecialAction; }
	
public:
	UCSubAction();

public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);
	virtual void Tick(float InDeltaTime) {}

public:
	virtual void Pressed();
	virtual void Released();

public:
	virtual bool TryGuard(ACBaseCharacter::FDamageData& DamageData);


public:
	UFUNCTION(BlueprintNativeEvent)
	void Begin_DoSubAction();
	virtual void Begin_DoSubAction_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void End_DoSubAction(); 
	virtual void End_DoSubAction_Implementation() {}

public:
	FSAGuardValueChanged OnGuardValueChanged;

protected:
	bool bInAction;
	bool bInSpecialAction; 

	class ACharacter* Owner; 
	class ACAttachment* Attachment;
	class UCDoAction* DoAction;

	class UCStateComponent* State;
	class UCMovementComponent* Movement;
	class UCWeaponComponent* Weapon;
};
