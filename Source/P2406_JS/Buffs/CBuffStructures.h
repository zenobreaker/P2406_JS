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
struct FStatBuffUIData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BuffID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor TintColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BuffName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString BuffDesc= "";
};


USTRUCT(BlueprintType)
struct FStatBuff
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BuffID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharStatType StatType = ECharStatType::Max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Duration = -1.0f;  // 지속 시간 (-1이면 무한 지속)

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FName BuffName = NAME_None;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//FString BuffDesc = "";

private:
	FTimerHandle TimerHandle; // 버프 만료 타이머

public:
	FStatBuff() {}
	FStatBuff(FName InBuffID, ECharStatType InType, float InValue, float InDuration = -1.0f)
		: BuffID(InBuffID), StatType(InType), Value(InValue), Duration(InDuration)
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
