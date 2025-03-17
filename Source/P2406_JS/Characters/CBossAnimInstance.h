#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CBoss_AI.h"
#include "CBossAnimInstance.generated.h"


UCLASS()
class P2406_JS_API UCBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	EBossState BossState;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Speed;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Direction;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Animation")
	float RotateSpeed;


public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private: 
	UFUNCTION()
	void OnBossStateChanged(EBossState InType);

private:
	class ACharacter* OwnerCharacter;
	class UCConditionComponent* Condition;

private:
	FRotator PrevRotation;
};
