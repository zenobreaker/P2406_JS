#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enums/FEnum.h"
#include "CConditionComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAddCondiitionType, EConditionState, InType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRemoveCondiitionType, EConditionState, InType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCConditionComponent : public UActorComponent
{
	GENERATED_BODY()
  

public:
    FORCEINLINE bool IsNoneCondition() { return Condition == EConditionState::CONDITION_NONE; }
    FORCEINLINE bool GetAirborneCondition() { return HasCondition(EConditionState::CONDITION_AIRBORNE); }
    FORCEINLINE bool GetDownCondition() { return HasCondition(EConditionState::CONDITION_DOWNED); }

public:	
	UCConditionComponent();

protected:
	virtual void BeginPlay() override;

public:
    UFUNCTION()
    void AddAirborneCondition();
    
    UFUNCTION()
    void RemoveAirborneCondition();

    UFUNCTION()
    void AddDownCondition();

    UFUNCTION()
    void RemoveDownCondition();

private:
    //void ChangeType(EConditionState InPrevType, EConditionState InNewType);

private:
    bool HasCondition(EConditionState InCondition);
    bool IsSameCondition(EConditionState InCondition1, EConditionState InCondition2);

private:
    void AddCondition(EConditionState NewCondition);
    void RemoveCondition(EConditionState NewCondition);

public:
    FAddCondiitionType OnAddCondiitionType;
    FRemoveCondiitionType OnRemoveCondiitionType;

private:
	EConditionState Condition;
    class ACharacter* OwnerCharacter; 
};
