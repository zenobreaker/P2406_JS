#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CBuffStructures.generated.h"


UENUM(BlueprintType)
enum class ECharStatType : uint8
{
	Attack, AttackSpeed, Defense, Speed, CriticalRate, CriticalDamage, Max,
};


USTRUCT(BlueprintType)
struct FStatBuff
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharStatType StatType = ECharStatType::Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = -1.0f;  // ���� �ð� (-1�̸� ���� ����)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IconPath = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BuffName = NAME_None; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuffDesc = "";

private:
	FTimerHandle TimerHandle; // ���� ���� Ÿ�̸�

public:
	FStatBuff() {}
	FStatBuff(ECharStatType InType, float InValue, float InDuration = -1.0f)
		: StatType(InType), Value(InValue), Duration(InDuration) 
	{
		SetBuffName(); 
		SetBuffDesc(); 
	}

	void SetBuffName();
	void SetBuffDesc();
};


UCLASS()
class P2406_JS_API UCBuffStructures : public UObject
{
	GENERATED_BODY()
	
};
