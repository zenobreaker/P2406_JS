// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/DataTable.h"
#include "CParkourComponent.generated.h"


UENUM(BlueprintType)
enum class EParkourArrowType : uint8
{
	Center = 0, Head, Foot, Left, Right, Land, Max,
};

UENUM(BlueprintType)
enum class EParkourType : uint8
{
	Climb = 0, Fall, Slide, Short, Normal, Wall, Max,
};


USTRUCT(BlueprintType)
struct FParkourData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EParkourType Type = EParkourType::Max;

	UPROPERTY(EditAnywhere)
	UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	FName SectionName;

	UPROPERTY(EditAnywhere)
	float MinDistance = 0.0f;

	UPROPERTY(EditAnywhere)
	float MaxDistance = 0.0f;

	UPROPERTY(EditAnywhere)
	float Extent = 0.0f;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera = false;

public:
	void PlayMontage(class ACharacter* InCharacter);
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class P2406_JS_API UCParkourComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* DataTable;


private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 600;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float AvailableFrontAngle = 15;

	UPROPERTY(EditAnywhere, Category = "Trace")
	TEnumAsByte<EDrawDebugTrace::Type> DebugType;

public:
	FORCEINLINE bool IsDoing() const {return Type != EParkourType::Max;}

public:

	UCParkourComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void DoParkour(bool bLanded = false);
	void End_DoParkour();

private:
	void LineTrace(EParkourArrowType InType);

private:
	void CheckTrace_Center();
	void CheckTrace_Head();
	void CheckTrace_Foot();
	void CheckTrace_Side();

	void CheckTrace_Land();

private:
	bool Check_Obstacle();

private:
	bool Check_ClimbMode();
	void DoParkour_Climb();
	void End_DoParkour_Climb();

	bool Check_FallMode();
	void DoParkour_Fall();
	void End_DoParkour_Fall();

	bool Check_SlideMode();
	void DoParkour_Slide();
	void End_DoParkour_Slide();

	bool Check_ObstacleMode(EParkourType InType, FParkourData& OutData);
	void DoParkour_Obstacle(EParkourType InType, FParkourData& InData);
	void End_DoParkour_Obstacle();

private:
	TMap<EParkourType, TArray<FParkourData>> DataMap;

private:
	class ACharacter* OwnerCharacter;
	class UArrowComponent* Arrows[(int32)EParkourArrowType::Max];

private:
	FHitResult HitResults[(int32)EParkourArrowType::Max];

private:
	EParkourType Type = EParkourType::Max;

private:
	AActor* HitObstacle;
	FVector HitObstacleExtent;
	float HitDistance;
	float ToFrontYaw;

private:
	AActor* BackupObstacle;

private:
	bool bFirstFalling;


};
