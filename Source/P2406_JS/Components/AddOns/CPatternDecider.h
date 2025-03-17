#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/CPatternComponent.h"
#include "CPatternDecider.generated.h"

UENUM(BlueprintType)
enum class EPatternDecision
{
	KeepPattern,	// ���� ����
	ChangePattern,		// �ٸ� ���� ����
	CanclePattern,		// ���� ���
};


// ������ �����ϰ� �����ϰ� �����ִ� Ŭ���� 
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
	int32 MaintainPatternID; // �����Ϸ��� ���� 
};
