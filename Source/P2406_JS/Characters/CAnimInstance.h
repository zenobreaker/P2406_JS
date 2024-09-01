#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance.generated.h"


UCLASS()
class P2406_JS_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Speed; 
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		bool bFalling;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
		float Pitch;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private:
	class ACharacter* OwnerCharacter; 

};
