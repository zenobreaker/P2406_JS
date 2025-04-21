#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CFeetComponent.generated.h"

USTRUCT(BlueprintType)
struct FFeetData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector LeftDistance; // X

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector RightDistance; // X 

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FVector PelvisDistance; // Z

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FRotator LeftRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Feet")
	FRotator RightRotation;
};

UCLASS(ClassGroup = (Additional), meta = (BlueprintSpawnableComponent, DisplayName = "FeetIK 컴포넌트"))
class P2406_JS_API UCFeetComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	UPROPERTY(EditAnywhere, Category = "Debug")
	bool bEnableDebug = false; 

private:
	UPROPERTY(EditAnywhere, Category = "Trace")
	float TraceDistance = 50.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float OffsetDistance = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Trace")
	float InterpSpeed = 50.0f;

private:
	UPROPERTY(EditAnywhere, Category = "Bone")
	FName LeftBoneName = "Foot_L";

	UPROPERTY(EditAnywhere, Category = "Bone")
	FName RightBoneName = "Foot_R";

public:
	FORCEINLINE const FFeetData& GetData() { return Data; }

public:
	UCFeetComponent();

protected:
	virtual void BeginPlay() override;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	void Trace(FName InName, float& OutDistance, FRotator& OutRotation);

private:
	class ACharacter* OwnerCharacter;

	FFeetData Data;
};
