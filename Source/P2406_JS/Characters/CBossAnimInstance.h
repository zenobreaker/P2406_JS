#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/CBoss_AI.h"
#include "Components/CFeetComponent.h"
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


protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	bool bUseFootIK;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	FFeetData FeetData;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	float LeftIKValue = 1.0f;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Feet")
	float RightIKValue = 1.0f;


public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

private: 
	UFUNCTION()
	void OnBossStateChanged(EBossState InType);

private:
	void ChangeFeet(); 
	float GetIKValue(float InValue1, float InValue2); 

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State; 
	class UCConditionComponent* Condition;
	class UCFeetComponent* Feet; 
private:
	FRotator PrevRotation;
};
