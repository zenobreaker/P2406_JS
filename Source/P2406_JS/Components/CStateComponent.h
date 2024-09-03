#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"

UENUM()
enum class EStateType : uint8
{
	Idle = 0, Evade , Equip, Damaged, Action, Dead, Max, 
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class P2406_JS_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsDamagedMode() { return Type == EStateType::Damaged; }
	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetIdleMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetActionMode();
	void SetDamagedMode();
	void SetDeadMode();

	void OnSubActionMode();
	void OffSubActionMode();

private:
	void ChangeType(EStateType InType);

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;

private:
	bool bInSubActionMode;
};
