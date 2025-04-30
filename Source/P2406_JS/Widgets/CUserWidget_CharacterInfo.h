#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CStatComponent.h"
#include "CUserWidget_CharacterInfo.generated.h"


USTRUCT(BlueprintType)
struct FStatusUIData
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	FText HPText; 

	UPROPERTY(BlueprintReadOnly)
	FText AttackText; 

	UPROPERTY(BlueprintReadOnly)
	FText AttackSpeedText; 

	UPROPERTY(BlueprintReadOnly)
	FText DefenseText; 

	UPROPERTY(BlueprintReadOnly)
	FText CritText; 

	UPROPERTY(BlueprintReadOnly)
	FText CritDmgText; 

	UPROPERTY(BlueprintReadOnly)
	FText SpeedText; 


	UPROPERTY(BlueprintReadOnly)
	FText BuffedAttackText; 

	UPROPERTY(BlueprintReadOnly)
	FText BuffedAttackSpeedText;

	UPROPERTY(BlueprintReadOnly)
	FText BuffedDefenseText;

	UPROPERTY(BlueprintReadOnly)
	FText BuffedCritText;

	UPROPERTY(BlueprintReadOnly)
	FText BuffedCritDmgText;

	UPROPERTY(BlueprintReadOnly)
	FText BuffedSpeedText;
};

UCLASS()
class P2406_JS_API UCUserWidget_CharacterInfo : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void DrawInfo(class ACharacter* InOwner);

	UFUNCTION(BlueprintCallable)
	void SetStat(class ACharacter* InOwer); 


private:
	FText GetValueToText(float InValue, bool bPercent);

public:
	UPROPERTY(BlueprintReadOnly)
	class UCStatComponent* Stat; 

public:
	struct FStatusData StatusInfo; 

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	struct FStatusUIData StatusUIInfo;
};
