#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"


UCLASS()
class P2406_JS_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Bending;

public:
	FORCEINLINE float* GetBending() { return &Bending; }

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
};
