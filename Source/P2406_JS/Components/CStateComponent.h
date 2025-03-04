#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums//FEnum.h"
#include "CStateComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS()
class P2406_JS_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE bool IsIdleMode() const { return Type == EStateType::Idle; }
	FORCEINLINE bool IsEquipMode() const { return Type == EStateType::Equip; }
	FORCEINLINE bool IsEvadeMode() const { return Type == EStateType::Evade; }
	FORCEINLINE bool IsDashMode() const { return Type == EStateType::Dash; }
	FORCEINLINE bool IsDeadMode() const { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() const { return Type == EStateType::Action; }
	FORCEINLINE bool IsDamagedMode() const { return Type == EStateType::Damaged; }
	FORCEINLINE bool IsAirborneMode() const { return Type == EStateType::Airborne; }
	FORCEINLINE bool IsDownMode() const { return Type == EStateType::Down; }
	FORCEINLINE bool IsGuardMode() const { return Type == EStateType::Guard; }
	FORCEINLINE bool IsExecuteMode() const { return Type == EStateType::Execute; }
	FORCEINLINE bool IsSubActionMode() const { return bInSubActionMode; }

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	void SetIdleMode();
	void SetDashMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetActionMode();
	void SetDamagedMode();
	void SetDeadMode();
	void SetAirborneMode();
	void SetDownMode();
	void SetGuardMode();
	void SetExecuteMode();

	FString EnumToString(EStateType State);

	void OnSubActionMode();
	void OffSubActionMode();

private:
	void ChangeType(EStateType InType);

	void HandleGuardState();

public:
	FStateTypeChanged OnStateTypeChanged;

private:
	EStateType Type;

private:
	bool bInSubActionMode;
};
