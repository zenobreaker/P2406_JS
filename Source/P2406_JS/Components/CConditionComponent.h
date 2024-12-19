#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CConditionComponent.generated.h"



UENUM(BlueprintType)
enum class EConditionState : uint8
{
    CONDITION_NONE = 0 UMETA(DisplayName = "None"),
    CONDITION_DOWNED = 1 << 0 UMETA(DisplayName = "Downed"),
    CONDITION_STRUNED = 1 << 1 UMETA(DisplayName = "Stunned"),
    CONDITION_FROZEN = 1 << 2 UMETA(DisplayName = "Frozen"),
    CONDITION_AIRBORNE = 1 << 3 UMETA(DisplayName = "Airborne"),
    // 필요 상태 추가
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class P2406_JS_API UCConditionComponent : public UActorComponent
{
	GENERATED_BODY()
  

public:	
	UCConditionComponent();

protected:
	virtual void BeginPlay() override;

public:
    void AddAirborneCondition();
    void RemoveAirborneCondition();

    void AddDownCondition();
    void RemoveDownCondition();

private:
    bool HasCondition(EConditionState InCondition);
    
    void AddCondition(EConditionState NewCondition);
    void RemoveCondition(EConditionState NewCondition);

private:
    EConditionState Condition; 
    class ACharacter* OwnerCharacter; 
};
