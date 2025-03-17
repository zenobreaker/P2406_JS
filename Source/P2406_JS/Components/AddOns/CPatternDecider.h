#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CPatternComponent.h"
#include "CPatternDecider.generated.h"

UENUM(BlueprintType)
enum class EPatternDecision
{
	KeepPattern,	// 패턴 유지
	ChangePattern,		// 다른 패턴 선택
	CanclePattern,		// 패턴 취소
};


// 패턴을 세밀하게 결정하게 도와주는 클래스 
UCLASS(Blueprintable)
class P2406_JS_API UCPatternDecider : public UObject
{
	GENERATED_BODY()

private:
	friend UCPatternComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Decider")
	float KeepPatternRatio = 0.2f; // 20%

public:
	UCPatternDecider();

public:
	void BeginPlay();


public:
	EPatternDecision EvaluatePattern() const;

private:
	int32 MaintainPatternID; // 유지하려는 패턴 
};
